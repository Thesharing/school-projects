#include <stdio.h>
#include <windows.h>
#include "types.h"
#include "prototypes.h"

//output.c

int openFileToWrite(char inFileName[], FILE * * fpout){
	//根据文件名打开输入输出文件 ,文件结构变量由fpout指向。打开成功，返回1，否则返回0
	if (((*fpout) = fopen(inFileName, "w+")) == NULL)
		return 0;
	else
		return 1;
}

void set_fileOutput(FILE *fpout, int time_count, struct ELEVATOR elevator, LISTNODEPTR headp, int detail){
	//将电梯状态输出至output.txt和outputDetail.txt 
	//fpout：指向要被写入的文件的结构变量，timeCount：此刻的时间片
	//detail:为1代表写入 outputDetail.txt中，为0代表写入 output.txt中
	char * str1[6] = { "IDLE", "UP", "DOWN", "DOOROPENING", "DOORCLOSING", "STOP" };//存储状态常量字符串的地址
	char * str2[5] = { "T", "U", "D", "o", "c" };//存储枚举常量字符串的地址
	LISTNODEPTR currentPtr = headp;//请求链表的头结点

	switch (detail){
	case (1) :
		//输入详细记录
		fprintf(fpout, "T=%3d,State=%11s,NowF=%d+%d,GoalF=%d,StopT=%d,WaitF=",
		time_count, str1[elevator.eleState], elevator.nowF, elevator.stateTime, elevator.goalF, elevator.stopTime);
		while (currentPtr->nextPtr != NULL){ //当链表不为空时，输出目标楼层与相应请求动作
			fprintf(fpout, "%d%s ", (currentPtr->nextPtr)->reqData.reqF,
				str2[(currentPtr->nextPtr)->reqData.reqAct]);
			currentPtr = currentPtr->nextPtr;//移到下一节点处
		}
		fprintf(fpout, "\n");//当前时刻请求记录完毕，进行换行处理
		break;
	case (2) :
		//输入状态变化记录
		fprintf(fpout, "T=%3d,State=%11s,NowF=%d,GoalF=%d,StopT=%d,WaitF=",
		time_count, str1[elevator.eleState], elevator.nowF, elevator.goalF, elevator.stopTime);
		while (currentPtr->nextPtr != NULL){ //当链表不为空时，输出 目标楼层与相应请求动作
			fprintf(fpout, "%d%s ", (currentPtr->nextPtr)->reqData.reqF,
				str2[(currentPtr->nextPtr)->reqData.reqAct]);
			currentPtr = currentPtr->nextPtr;//移到下一节点处
		}
		fprintf(fpout, "\n");//当前时刻请求记录完毕，进行换行处理
		break;
	}
}

//end of output.c