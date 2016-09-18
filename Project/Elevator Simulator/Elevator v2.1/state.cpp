#include <stdio.h>
#include <windows.h>
#include "types.h"
#include "prototypes.h"

extern HANDLE hMutex; //定义互斥变量 

//state.c

/*计算电梯运行的状态*/
//struct REQ * reqTempPtr：暂时存放已经进入目标楼层的请求，用以屏幕输出。
void runService(struct ELEVATOR * elevator, LISTNODEPTR headp, struct REQ * reqTempPtr){
	int actionUsed = 0; //如果命令被执行则在函数尾删除命令
	struct REQ actionTemp;//临时请求结构，存放链表中第一个请求
	LISTNODEPTR nowPtr;//链表的当前位置

	nowPtr = headp->nextPtr; //将nowPtr初始化 
	if (nowPtr == NULL) //如果没有命令则将临时请求数组标识为空 
		actionTemp.reqAct = EMPTY;
	else{//有命令时，将当前节点指向的结构变量赋给临时请求结构
		actionTemp = nowPtr->reqData;
	}

	switch (elevator->eleState){ //判断电梯状态 
	case (IDLE) : //空闲 
		if (actionTemp.reqAct != -1){
			if (actionTemp.reqAct == OPEN || actionTemp.reqAct == CLOSE){//当前命令为开关门请求
				if (actionTemp.reqAct == OPEN){//开门请求
					elevator->eleState = DOOROPENING;//修改电梯状态
					elevator->stateTime = OPENLEN;//修改当前状态时间
				}
			}
			else if (actionTemp.reqF > elevator->nowF){ //上升 
				elevator->eleState = UP;//修改电梯状态
				elevator->goalF = actionTemp.reqF;//修改电梯的目标楼层
				elevator->stateTime = ONELAYERLEN;//修改当前状态时间
			}
			else if (actionTemp.reqF < elevator->nowF){ //下降 
				elevator->eleState = DOWN;//修改电梯状态
				elevator->goalF = actionTemp.reqF;//修改电梯的目标楼层
				elevator->stateTime = ONELAYERLEN;//修改当前状态时间
			}
			else if (actionTemp.reqF == elevator->nowF){ //本层命令 
				elevator->eleState = DOOROPENING;//修改电梯状态
				elevator->stateTime = OPENLEN;//修改当前状态时间
			}
			actionUsed = 1;//当前命令被执行，将变量赋为1
		}
		(*reqTempPtr).reqAct = EMPTY;//初始化reqTempPtr指向的结构
		(*reqTempPtr).reqF = 0;
		break;
	case (DOOROPENING) : //开门 
		if (actionTemp.reqAct == CLOSE){ //按下关门键，则按照已开门时间关门 
			elevator->eleState = DOORCLOSING;//修改电梯状态
			elevator->stateTime = OPENLEN - elevator->stateTime;//修改当前状态时间
			actionUsed = 1;//当前命令被执行，将变量赋为1
		}
		else if (elevator->stateTime == 0){ //开门完毕 
			elevator->eleState = STOP;//修改电梯状态
			elevator->stateTime = STOPLEN;//修改当前状态时间
		}
		else{ //开门未完成 
			elevator->stateTime--;//修改当前状态时间，状态时间减一
		}
		break;
	case (STOP) : //停靠
		if (actionTemp.reqAct == OPEN){ //按下开门键，等候时间重置 
			elevator->stateTime = STOPLEN;//修改当前状态时间
			actionUsed = 1;//当前命令被执行，将变量赋为1
		}
		else if (actionTemp.reqAct == CLOSE || elevator->stateTime == 0){ //按下关门键或者等候时间到 
			elevator->eleState = DOORCLOSING;//修改电梯状态
			elevator->stateTime = CLOSELEN;//修改当前状态时间
			if (actionTemp.reqAct == CLOSE)
				actionUsed = 1;//当前命令被执行，将变量赋为1
		}
		else//等候时间未到
			elevator->stateTime--; //修改当前状态时间，状态时间减一
		break;
	case (DOORCLOSING) : //关门 
		if (actionTemp.reqAct == OPEN){ //按下开门键，按照已关门时间开门 
			elevator->eleState = DOOROPENING;//修改电梯状态
			elevator->stateTime = CLOSELEN - elevator->stateTime;//修改当前状态时间
			actionUsed = 1;
		}
		else if (actionTemp.reqAct == CLOSE){
			elevator->eleState = DOORCLOSING;//避免关门后再按关门键 
			actionUsed = 1;//当前命令被执行，将变量赋为1
		}
		else if (elevator->stateTime == 0){ //已经关门 
			if (actionTemp.reqAct != -1){ //如果有命令 
				if (actionTemp.reqF > elevator->nowF){ //上升 
					elevator->eleState = UP;//修改电梯状态
					elevator->goalF = actionTemp.reqF;//修改电梯的目标楼层
					elevator->stateTime = ONELAYERLEN;//修改当前状态时间
				}
				else if (actionTemp.reqF == elevator->nowF){ //本楼层响应 
					elevator->eleState = DOOROPENING;//修改电梯状态
					elevator->stateTime = OPENLEN;//修改当前状态时间
				}
				else if (actionTemp.reqF<elevator->nowF){  //下降 
					elevator->eleState = DOWN;//修改电梯状态
					elevator->goalF = actionTemp.reqF;//修改电梯的目标楼层
					elevator->stateTime = ONELAYERLEN;//修改当前状态时间
				}
				actionUsed = 1;//当前命令被执行，将变量赋为1
			}
			else
				elevator->eleState = IDLE;//如果没有请求则进入空闲
		}
		else {
			elevator->stateTime--; //关门未结束时 ，状态时间减一
		}
		break;
	case (UP) : //向上 
		if (elevator->stateTime != 0) //当位于两层之间时 
			elevator->stateTime--;//修改当前状态时间，状态时间减一
		else if ( elevator->nowF < elevator->goalF ){ //当未到达指定楼层时 
			elevator->nowF ++;//修改电梯当前楼层
			if ( elevator->nowF != elevator->goalF )//当前楼层还是不为目标楼层时
				elevator->stateTime = ONELAYERLEN;//修改当前状态时间，继续上行
		}
		else{ //当到达指定楼层时 
			elevator->eleState = DOOROPENING;//修改电梯状态
			elevator->stateTime = OPENLEN;//修改当前状态时间
		}
		break;
	case (DOWN) : //向下 
		if (elevator->stateTime != 0)//当位于两层之间时 
			elevator->stateTime--;//修改当前状态时间，状态时间减一
		else if (elevator->nowF > elevator->goalF){//当未到达指定楼层时 
			elevator->nowF--;//修改电梯当前楼层
			if (elevator->nowF != elevator->goalF )//当前楼层还是不为目标楼层时
				elevator->stateTime = ONELAYERLEN;//修改当前状态时间，继续下行
		}
		else{
			elevator->eleState = DOOROPENING;//修改电梯状态
			elevator->stateTime = OPENLEN;//修改当前状态时间
		}
		break;
	}
	if (actionUsed == 1){ //当命令被执行时即删去 
		(* reqTempPtr).reqAct = actionTemp.reqAct;//将删去的命令存到reqTempPtr指向的结构中
		(* reqTempPtr).reqF = actionTemp.reqF;
		deleteListNode(headp);//删除节点
	}
}

 /*删除节点*/
//listPtr是要删除的节点前面一个节点的地址
void deleteListNode(LISTNODEPTR listPtr){
	LISTNODEPTR followPtr;
	followPtr = listPtr->nextPtr;
	listPtr->nextPtr = followPtr->nextPtr;
	free(followPtr);
}

/*对电梯进行初始化*/
void initElevator(struct ELEVATOR * elePtr){  
	elePtr->eleState = IDLE;
	elePtr->nowF = 1;
	elePtr->goalF = 1;
	elePtr->stopTime = 0;
	elePtr->stateTime = 0;
}

//对开关门请求的响应
//req==1：关门请求 req==2:开门请求
void doorReqFunc(int req, struct ELEVATOR * elevatorPtr){
	if (req == 1){//开门请求
		if (elevatorPtr->eleState == STOP){//在电梯停靠状态遇到开门请求再增加一个停靠的时间片
			elevatorPtr->stateTime = STOPLEN;
		}
		else if (elevatorPtr->eleState == DOORCLOSING){//电梯正在关门遇到开门请求，在已关的门的基础上开门
			elevatorPtr->eleState = DOOROPENING;
			elevatorPtr->stateTime = CLOSELEN-elevatorPtr->stateTime;
		}
		else if (elevatorPtr->eleState == IDLE){//电梯处于空闲状态，直接响应开门请求
			elevatorPtr->eleState = DOOROPENING;
			elevatorPtr->stateTime = OPENLEN;
		}
	}
	else if (req == 2){//关门请求
		if (elevatorPtr->eleState == STOP){//在电梯停靠状态遇到关门请求，直接响应关门请求
			elevatorPtr->eleState = DOORCLOSING;
			elevatorPtr->stateTime = CLOSELEN;
		}
		else if (elevatorPtr->eleState == DOOROPENING){//电梯正在关门遇到开门请求，在已开的门的基础上关门
			elevatorPtr->eleState = DOORCLOSING;
			elevatorPtr->stateTime = elevatorPtr->stateTime;
		}
	}
}
//end of state.c