#include <stdio.h> 
#include <windows.h>
#include "graphics.h"
#include "types.h"
#include "prototypes.h"

extern HANDLE hMutex; //定义互斥变量 

//input.c

DWORD WINAPI keyboardInputProc(LPVOID lpParameter){ 
	//键盘模式：创建键盘读取进程 
	struct REQ reqs[10];//因为addServList函数的格式因而将reqs定义为数组，同时将reqCount始终定义为1 
	reqs[0].reqAct = EMPTY;//初始化
	reqs[0].reqF = 1;
	int reqCount=1;
	struct HANDLETRANS * handleTransPtr = (struct HANDLETRANS *) lpParameter;//将传入的链表格式化 
		LISTNODEPTR headp = (* handleTransPtr).headp;//获取链表头结点的地址
		struct ELEVATOR * elevatorPtr = (* handleTransPtr).elevatorPtr;//获取电梯状态的地址
		int * serviceMethodPtr =  (* handleTransPtr).serviceMethodPtr;//获取策略模式的地址
		struct REQ * reqTempPtr =(* handleTransPtr).reqTempPtr;//获取临时请求结构的地址

	while (1){ //无限循环 
		keyboardInput(reqs,serviceMethodPtr); //调用键盘输入函数等待读取键盘输入 
		if (reqs[0].reqAct != EMPTY){
			WaitForSingleObject(hMutex, INFINITE);//当用户有键盘输入时，申请互斥变量 
			addServList((*serviceMethodPtr), headp, reqs, reqCount, elevatorPtr, reqTempPtr);//将读取到的请求加入链表 
			ReleaseMutex(hMutex);//释放互斥变量
		}
	}
}

/*将读取的字符请求进行转换存入请求数组中*/
void keyboardInput(struct REQ reqs[], int * serviceMethodPtr){ 
	char ch;
	ch = getch(); //读取字符 
	if(ch <= '9' && ch >= '1'){ //当字符是1-9时，识别请求为内部请求 
		reqs[0].reqAct = INSIDE;
		reqs[0].reqF = (int)ch-48;
	}
	else if (ch == 'z' || ch == 'x'){ //当字符是z和x时，识别为开门关门请求 
		if(ch == 'z'){
			reqs[0].reqAct = OPEN;
			reqs[0].reqF = 0;
		}
		else {
			reqs[0].reqAct = CLOSE;
			reqs[0].reqF = 0;
		}
	}
	else {
		switch (ch){ //对其余字符进行识别 
			case ('q')://8 qwertyui对应的是外部上行 
				reqs[0].reqAct = OUTSIDEUP;
				reqs[0].reqF = 8;
				break;
			case ('w'):case ('t')://7&4
				reqs[0].reqAct = OUTSIDEUP;
				reqs[0].reqF = (int)ch-112;
				break;
			case ('e')://6
				reqs[0].reqAct = OUTSIDEUP;
				reqs[0].reqF = 6;
				break;
			case ('r')://5
				reqs[0].reqAct = OUTSIDEUP;
				reqs[0].reqF = 5;
				break;
			case ('y')://3
				reqs[0].reqAct = OUTSIDEUP;
				reqs[0].reqF = 3;
				break;
			case ('u')://2
				reqs[0].reqAct = OUTSIDEUP;
				reqs[0].reqF = 2;
				break;
			case ('i')://1
				reqs[0].reqAct = OUTSIDEUP;
				reqs[0].reqF = 1;
				break;
			case ('a')://9 asdfghj对应的是外部下行 
				reqs[0].reqAct = OUTSIDEDOWN;
				reqs[0].reqF = 9;
				break;
			case ('s')://8
				reqs[0].reqAct = OUTSIDEDOWN;
				reqs[0].reqF = 8;
				break;
			case ('d')://7
				reqs[0].reqAct = OUTSIDEDOWN;
				reqs[0].reqF = 7;
				break;
			case ('f')://6
				reqs[0].reqAct = OUTSIDEDOWN;
				reqs[0].reqF = 6;
				break;
			case ('g')://5
				reqs[0].reqAct = OUTSIDEDOWN;
				reqs[0].reqF = 5;
				break;
			case ('h')://4
				reqs[0].reqAct = OUTSIDEDOWN;
				reqs[0].reqF = 4;
				break;
			case ('j')://3
				reqs[0].reqAct = OUTSIDEDOWN;
				reqs[0].reqF = 3;
				break;
			case ('k')://2
				reqs[0].reqAct = OUTSIDEDOWN;
				reqs[0].reqF = 2;
				break;
			case ('c')://更换为先来先服务策略
				(*serviceMethodPtr) = 1;
				break;
			case ('v')://更换为顺便服务策略
				(*serviceMethodPtr) = 2;
				break;
			case ('b') ://更换为宏福电梯策略
				(*serviceMethodPtr) = 3;
				break;
			case ('m') ://退出
				exit(0);
		}
	}
}

/*鼠标模式：创建鼠标读取进程*/
DWORD WINAPI mouseInputProc(LPVOID lpParameter){
	struct REQ reqs[10]; //因为addServList函数的格式因而将reqs定义为数组，同时将reqCount始终定义为1
	reqs[0].reqAct = EMPTY;//为了防止切换策略时请求错误将请求数组的第一个初始化
	reqs[0].reqF = 1;
	int reqCount = 1;//固定每一次获得的请求为1个
	struct HANDLETRANS * handleTransPtr = (struct HANDLETRANS *) lpParameter; //将传入的链表格式化 
		LISTNODEPTR headp = (*handleTransPtr).headp;//获取链表头结点的地址
		struct ELEVATOR * elevatorPtr = (*handleTransPtr).elevatorPtr;//获取电梯状态的地址
		int * serviceMethodPtr = (*handleTransPtr).serviceMethodPtr;//获取策略模式的地址
		struct REQ * reqTempPtr = (*handleTransPtr).reqTempPtr;//获取临时存储请求的地址
	while (1){ //无限循环 
		for (; is_run(); delay_fps(60)){
			mouseInput(reqs, serviceMethodPtr);//调用键盘输入函数等待读取键盘输入 
			if (reqs[0].reqAct != EMPTY){//防止当切换策略时没有请求而导致请求读入出错
				WaitForSingleObject(hMutex, INFINITE); //当用户有键盘输入时，申请互斥变量 
				addServList((*serviceMethodPtr), headp, reqs, reqCount, elevatorPtr, reqTempPtr); //将读取到的请求加入链表
				ReleaseMutex(hMutex);//释放互斥变量
			}//if
		}//for
	}//while
}

void mouseInput(struct REQ reqs[], int * serviceMethodPtr){
	//获取鼠标消息，这个函数会等待，等待到有消息为止
	mouse_msg msg = { 0 };//初始化鼠标信息变量
	msg = getmouse();//第一次获得鼠标信息
	while ((int)msg.is_down() ==0)//直到鼠标被按下时才跳出循环
	{
		msg = getmouse();
	}
	if ((int)msg.is_down() == 1){//当鼠标被按下时
		if (msg.x >= 576 && msg.x < 624){//内部请求（6-9楼）或开门键
			if (msg.y >= 240 && msg.y < 432){//内部请求（6-9楼）
				reqs[0].reqAct = INSIDE;
				reqs[0].reqF = 9 - (((int)msg.y - 240) / 48);
			}
			else if (msg.y >= 192 && msg.y < 240){//开门键
				reqs[0].reqAct = OPEN;
				reqs[0].reqF = 0;
			}
		}
		else if (msg.x >= 624 && msg.x < 672){//内部请求（1-5楼）或关门键
			if (msg.y >= 336 && msg.y < 576){//内部请求（1-5楼）
				reqs[0].reqAct = INSIDE;
				reqs[0].reqF = 5 - (((int)msg.y - 336) / 48);
			}
			else if (msg.y >= 192 && msg.y < 240){//关门键
				reqs[0].reqAct = CLOSE;
				reqs[0].reqF = 0;
			}
		}
		else if (msg.x>192 && msg.x < 240){//外部向上
			if (msg.y >= 144 && msg.y < 528){
				reqs[0].reqAct = OUTSIDEUP;
				reqs[0].reqF = 8 - (((int)msg.y - 144) / 48);
			}
		}
		else if (msg.x>240 && msg.x < 288){//外部向下
			if (msg.y >= 96 && msg.y < 480){
				reqs[0].reqAct = OUTSIDEDOWN;
				reqs[0].reqF = 9 - (((int)msg.y - 96) / 48);
			}
		}
		else if (msg.x>336 && msg.x < 528){//切换至先来先服务
			if (msg.y >= 288 && msg.y <= 336){
				(*serviceMethodPtr) = 1;
			}
			else if (msg.y > 336 && msg.y <= 384){//切换至顺便服务
				(*serviceMethodPtr) = 2;
			}
			else if (msg.y > 384 && msg.y <= 432){
				(*serviceMethodPtr) = 3;
			}
		}
	}
}

//end of input.c