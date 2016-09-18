#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "protocol.h"
#include "datalink.h"

#define DATA_TIMER  5000 //Data帧超时时间
#define ACK_TIMER 280 //Ack帧超时时间

#define MAX_SEQ 31 //帧的序号空间，应当是2^n-1
#define NR_BUFS 16 //窗口大小，NR_BUFS=(MAX_SEQ+1)/2
#define inc(k) if(k < MAX_SEQ)k++;else k=0 //计算k+1

struct FRAME { //帧的内容
	unsigned char kind; //帧的种类，分为Ack, Nak 和 Data 三种
	unsigned char ack; //上一次成功接收的帧的序号
	unsigned char seq; //本帧的序号
	unsigned char data[PKT_LEN]; //数据
	unsigned int  padding; //填充字段
};

static unsigned char next_frame_to_send = 0, frame_expected = 0, ack_expected = 0;
//next_frame_to_send:将要发送的帧序号，发送方窗口的上界；frame_expected:期望收到的帧序号，接收方窗口的下界；ack_expected:期望收到的ack帧序号，发送方窗口的下界
static unsigned char out_buffer[NR_BUFS][PKT_LEN], in_buffer[NR_BUFS][PKT_LEN], nbuffered;
//对应输出缓存、输入缓存，以及目前输出缓存的个数
static unsigned char too_far = NR_BUFS; //接收方窗口的上界
static int phl_ready = 0; //物理层是否ready
bool arrived[NR_BUFS]; //接收方输入缓存的bit map
bool no_nak = true; //是否发送了NAK，true则不再重复发送

static bool between(int a, int b, int c){ //如果b在a和c组成的窗口之间，则返回true，否则返回false
	return((a <= b) && (b < c)) || ((c < a) && (a <= b)) || ((b < c) && (c <a));
}

static void put_frame(unsigned char *frame, int len){ 
	//将frame做CRC校验后附校验码一起发送到物理层
	*(unsigned int *)(frame + len) = crc32(frame, len);
	send_frame(frame, len + 4);
	phl_ready = 0; //将数据发送到物理层后将物理层置为忙碌状态
}

static void send_data_frame(unsigned char frame_nr){
	//将输出缓存打包成frame后调用put_frame发送到物理层，同时开始计时data_timer
	struct FRAME s;

	s.kind = FRAME_DATA;
	s.seq = frame_nr;
	s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);
	memcpy(s.data, out_buffer[frame_nr % NR_BUFS], PKT_LEN); //将out_buffer里的缓存复制到帧frame的data域中
	
	dbg_frame("Send DATA %d %d, ID %d\n", s.seq, s.ack, *(short *)s.data);
	
	put_frame((unsigned char *)&s, 3 + PKT_LEN); //将打包好的帧发送到物理层
	
	start_timer(frame_nr % NR_BUFS, DATA_TIMER);
	stop_ack_timer();
}

static void send_ack_frame(void){
	//打包ack确认帧并发送到物理层
	struct FRAME s;

	s.kind = FRAME_ACK;
	s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);

	dbg_frame("Send ACK  %d\n", s.ack);

	put_frame((unsigned char *)&s, 2);
	stop_ack_timer();
}

static void send_nak_frame(void){
	//打包nak确认帧并发送到确认帧
	struct FRAME s;

	s.kind = FRAME_NAK;
	s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);

	no_nak = false;

	dbg_frame("Send NAK with ACK %d\n", s.ack);

	put_frame((unsigned char *)&s, 2);
	stop_ack_timer();
}

int main(int argc, char **argv)
{
	//初始化
	nbuffered = 0; //初始没有输出帧被缓存
	int i;
	for (i = 0; i < NR_BUFS; i++) //初始时没有输入帧被缓存
		arrived[i] = false;
	int event, oldest_frame;
	struct FRAME f;
	int len = 0; //收到data的长度

	protocol_init(argc, argv); //协议初始化
	lprintf("Designed by Hong Zhilong, based on Teacher Jiang's code.\nBuild: "__DATE__"  "__TIME__"\n");

	disable_network_layer();

	for (;;)
	{
		event = wait_for_event(&oldest_frame);

		switch (event) //对不同事件进行不同处理
		{
			case NETWORK_LAYER_READY: //当网络层ready时
				get_packet(out_buffer[next_frame_to_send % NR_BUFS]); //从网络层获取一帧放入输出缓存中
				nbuffered++; //缓存序号+1
				send_data_frame(next_frame_to_send);//发送该帧
				inc(next_frame_to_send); //将将发送方窗口上限+1
				break;

			case PHYSICAL_LAYER_READY: //当物理层ready时，将phy_ready置为1以便之后enable网络层
				phl_ready = 1;
				break;

			case FRAME_RECEIVED: //当物理层收到一帧时
				len = recv_frame((unsigned char *)&f, sizeof f); //从物理层获取一帧
				if (len < 5 || crc32((unsigned char *)&f, len) != 0) { //如果接收坏帧或CRC校验失败，则返回nak帧
					dbg_event("**** Receiver Error, Bad CRC Checksum\n");
					if (no_nak)
						send_nak_frame();
					break;
				}
				if (f.kind == FRAME_ACK) //如果是ack帧的话，由于所有帧都含有ack帧，因此统一处理
					dbg_frame("Recv ACK  %d\n", f.ack);

				if (f.kind == FRAME_DATA) //如果是data帧
				{
					if ((f.seq != frame_expected) && no_nak) //如果收到的是不需要的帧则返回nak
						send_nak_frame();
					else
						start_ack_timer(ACK_TIMER);
					if (between(frame_expected, f.seq, too_far) && (arrived[f.seq%NR_BUFS] == false))
					//如果收到的帧在接收方窗口内且这一帧未被接收过
					{
						dbg_frame("Recv DATA %d %d, ID %d\n",f.seq, f.ack, *(short *)f.data);
						arrived[f.seq%NR_BUFS] = true; //标记该帧为已接受
						memcpy(in_buffer[f.seq % NR_BUFS], f.data,PKT_LEN); //将接收到的帧的data域复制至输入缓存中
						while (arrived[frame_expected % NR_BUFS])//当收到接收方窗口下界的一帧时，对这一帧以及之后收到的帧进行处理
						{
							put_packet(in_buffer[frame_expected %NR_BUFS], len - 7); //将输入缓存送至网络层
							no_nak = true;
							arrived[frame_expected % NR_BUFS] = false;
							inc(frame_expected); //将窗口前移一位
							inc(too_far);
							start_ack_timer(ACK_TIMER); //如果ack_timer超时则发送ack帧
						}
					}
				}

				if ((f.kind == FRAME_NAK) && between(ack_expected, (f.ack + 1) % (MAX_SEQ + 1), next_frame_to_send))
				//如果收到的是nak帧且ack的下一帧在发送方窗口里，则发送ack的下一帧
				{
					send_data_frame((f.ack + 1) % (MAX_SEQ + 1));
					dbg_frame("Recv NAK with ACK %d\n", f.ack);
				}

				while (between(ack_expected, f.ack, next_frame_to_send))
				//累计确认，只要ack在发送方窗口内就不断地将窗口前移直至未确认的一帧
				{
					nbuffered--;
					stop_timer(ack_expected%NR_BUFS);
					inc(ack_expected);
				}
				break;

			case ACK_TIMEOUT://ack_timer超时时发送独立的ack帧
				dbg_event("---- DATA %d timeout\n", oldest_frame);
				send_ack_frame();
				break;

			case DATA_TIMEOUT: //某帧的data_timer超时说明未收到接收方的ack帧，则重新发送该帧
				dbg_event("---- DATA %d timeout\n", oldest_frame);
				if (!between(ack_expected, oldest_frame, next_frame_to_send))
					oldest_frame = oldest_frame + NR_BUFS;
				send_data_frame(oldest_frame);
				break;
		}

		if (nbuffered < NR_BUFS && phl_ready) //如果物理层ready且目前缓存未满，则使网络层ready
			enable_network_layer();
		else
			disable_network_layer();
	}
}
