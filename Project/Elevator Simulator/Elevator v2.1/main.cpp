#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "graphics.h"
#include "types.h"
#include "prototypes.h"

HANDLE hMutex; //定义互斥变量

int main(void)
{
	//进行变量的定义和初始化
	int timeCount = 0;  //计时器，每循环一次模拟1s
	struct REQ reqs[20];  //暂存每次获得的某时刻的全部请求事件
	struct REQ reqTemp; //针对屏幕输出而设置的请求暂存，将从链表删除的请求复制到屏幕输出中，使得全部请求都可以显示
	int reqCount, reqTime;// //暂存每次获得的某时刻的请求事件总数以及请求的时间
	int inputMethod = 2, serviceMethod = 1; //输入模式和策略模式，初始为键盘输入和先来先服务策略
	enum STATE  preState;//存储电梯前一状态 
	LISTNODEPTR headp;//存请求队列链表头指针
	FILE *fpout, *fpout2;  //指向请求文件和输出文件File结构
	struct ELEVATOR elevator;//存储电梯对象信息，包括电梯状态、所在楼层、停靠时间等等
	reqs[0].reqAct = EMPTY;//将第一个请求初始化为EMPTY以防止在IDLE切换策略时进行addServList计算出现负值
	reqs[0].reqF = 1;

	//设置请求链表头节点。该头节点不实际存储请求
	headp = (LISTNODEPTR)malloc(sizeof(LISTNODE));
	(headp->reqData).reqAct = EMPTY;//将头结点的数据进行初始化 
	(headp->reqData).reqF = 0;
	headp->nextPtr = NULL;

	//初始化进程间地址传递的结构变量
	struct HANDLETRANS handleTrans;{
		handleTrans.headp = headp;//请求链表头结点
		handleTrans.elevatorPtr = &elevator;//电梯状态的地址
		handleTrans.serviceMethodPtr = &serviceMethod;//策略模式
		handleTrans.reqCountPtr = &reqCount;//请求数
		handleTrans.reqTimePtr = &reqTime;//请求发生的时间
		handleTrans.reqTempPtr = &reqTemp;//临时保存的请求
	}

	//初始化动画界面并设置背景色
	initgraph(768, 672);
	setbkcolor(WHITE);

	//创建键盘输入的进程
	HANDLE hThread1; //创建句柄 
	hMutex=CreateMutex(NULL,FALSE,NULL); //创建互斥变量 
	hThread1=CreateThread(NULL,0,keyboardInputProc,&handleTrans,0,NULL); //创建键盘输入进程，将请求链表传入 
	CloseHandle(hThread1); //关闭句柄

	//打开输出文件
	//根据文件名打开输出文件output.txt，当电梯状态有变化时就会往文件中输出   
	if (!openFileToWrite("output.txt", &fpout)){
		printf("Output file \"output.txt \" can't be opened.\n");
		return 0;
	}
	//根据文件名打开输出文件outputDetail.txt，定期往文件中输出电梯信息
	if (!openFileToWrite("outputDetail.txt", &fpout2)){//根据文件名打开输入输出文件   
		printf("Output file \"outputDetail.txt \" can't be opened.\n");
		return 0;
	}

	//初始化电梯结构信息，电梯初始位于1层，为空闲状态。 
	initElevator(&elevator);

	//开始模拟电梯运行
	//创建进程

	//建立 图像输出 的进程
	HANDLE hThread3; //创建句柄 
	hThread3 = CreateThread(NULL, 0, graphicOutProc, &handleTrans, 0, NULL); //创建图像输出进程，将请求链表传入 
	CloseHandle(hThread3);//关闭句柄

	//建立 计算电梯请求 的进程 （为顺便服务）
	HANDLE hThread2; //创建句柄 
	hThread2=CreateThread(NULL,0,addServListProc,&handleTrans,0,NULL); //创建键盘输入进程，将请求链表传入 
	CloseHandle(hThread2);//关闭句柄

	//建立 鼠标输入 的进程
	HANDLE hThread4; //创建句柄 
	hThread4 = CreateThread(NULL, 0, mouseInputProc, &handleTrans, 0, NULL); //创建鼠标输入进程，将请求链表传入 
	CloseHandle(hThread4);

	//进入主程序的循环，不断计算电梯状态，然后将电梯状态输出至文件中
	while (1){
		//1.处理输入请求在键盘进程中 
		preState = elevator.eleState;//记录前一状态
		WaitForSingleObject(hMutex, INFINITE); //等待互斥变量相应 
		runService(&elevator, headp, &reqTemp);//根据电梯当前状态计算下一状态
		ReleaseMutex(hMutex);//释放互斥变量 
		//2.输出电梯状态
		//输出电梯状态信息到文件2中
		WaitForSingleObject(hMutex, INFINITE);//申请互斥变量
		set_fileOutput(fpout2, timeCount, elevator, headp, 1);//输出电梯记录到指针fpout2指向的文件中
		if (elevator.eleState != preState)//当状态变化，将当前时间、状态和等待队列的情况写入到文件中
			set_fileOutput(fpout, timeCount, elevator, headp, 2);
		ReleaseMutex(hMutex);//释放互斥变量
		//3.时间片加1
		timeCount++;
		Sleep(1000);//等待1000ms
	}//End of while

	//关闭输出文件
	fclose(fpout);
	fclose(fpout2);

	system("pause");
	return 0;
}//end main
