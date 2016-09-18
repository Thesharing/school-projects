#include <stdio.h>
#include <windows.h>
#include "graphics.h"
#include "types.h"
#include "prototypes.h" 

extern HANDLE hMutex;//引入互斥变量

//strategy.c


//将请求根据策略不同，插入请求链表的合适位置 
// reqMode:请求模式，headPtr：指向存储请求的链表 头节点，reqs：存储请求的结构数组，reqsSize：reqs数组元素的个数，elevator：电梯结构变量，reqTempPtr：临时存放的请求结构
void addServList(int reqMode, LISTNODEPTR headPtr, struct REQ * reqs, int reqsSize,struct ELEVATOR * elevatorPtr, struct REQ * reqTempPtr)
{
	//定义并初始化变量
	LISTNODEPTR currentPtr = headPtr->nextPtr, tempPtr = NULL, previousPtr = headPtr; //初始化链表 
	struct REQ reqTemp; //临时结构
	int count = 0;//链表节点计数 
	int is_overlap=0;//判断请求是否重复，0为未重复，1为重复 
	//将请求数组里的请求加入请求链表中
	switch (reqMode){
		case (1) ://先来先服务策略
			while (count<reqsSize){//还未将请求数组中所有的请求放入链表中，继续循环
				if (reqs[count].reqAct != -1){//当请求不为空时
					tempPtr = (LISTNODEPTR)malloc(sizeof(LISTNODE)); //分配结点内存
					if (tempPtr != NULL){ //插入结点
						tempPtr->reqData = reqs[count];//把第i个请求赋给tempPtr指向的结构 
						if (reqs[count].reqAct == OPEN || reqs[count].reqAct == CLOSE){//如果是开门关门指令 
							if (reqs[count].reqAct == OPEN)//开门请求
								doorReqFunc(1, elevatorPtr);
							else//关门请求
								doorReqFunc(2, elevatorPtr);
						}//OPEN　& CLOSE
						else{//查看是否有与现有请求重复的节点 
							LISTNODEPTR lastPtr = headPtr;
							LISTNODEPTR nowPtr = headPtr->nextPtr;//初始化
							while (nowPtr != NULL){
								if (nowPtr->reqData.reqAct == tempPtr->reqData.reqAct
									&& nowPtr->reqData.reqF == tempPtr->reqData.reqF){//如果有重复请求 
									is_overlap = 1;//代表有重复请求出现
									LISTNODEPTR T = nowPtr;//暂存重复请求的节点 
									lastPtr->nextPtr = T->nextPtr;
									free(T);//释放重复请求的节点 
									break;
								}
								else{//此节点不重复
									lastPtr = nowPtr;//指针后移 
									nowPtr = nowPtr->nextPtr;
								}
							}
							if (is_overlap == 0){//如果整个链表中没有重复节点
								if (reqs[count].reqAct == INSIDE){//内部请求优先
									tempPtr->reqData = reqs[count];//把第i个请求赋给tempPtr指向的结构 
									if (headPtr->nextPtr == NULL){//链表内没有请求时 直接接上 
										headPtr->nextPtr = tempPtr;
										tempPtr->nextPtr = NULL;//tempPtr指向的下个节点为空 
									}
									else{//链表中有请求
										while (currentPtr != NULL&&currentPtr->reqData.reqAct != OUTSIDEUP
											&&currentPtr->reqData.reqAct != OUTSIDEDOWN){
											//currentPtr与previousPtr依次往后移动
											previousPtr = currentPtr;
											currentPtr = currentPtr->nextPtr; 
										}
										tempPtr->nextPtr = currentPtr;//暂存节点即内部请求连到外部请求之前 
										previousPtr->nextPtr = tempPtr;
									}
								}
								else{ //外部请求
									tempPtr->nextPtr = NULL;//防止访问地址有误
									if (currentPtr != NULL){
										while ((currentPtr->nextPtr) != NULL){//将指针移到链表尾部 
											currentPtr = currentPtr->nextPtr;
										}
										currentPtr->nextPtr = tempPtr;//将请求连到链表尾部 
										currentPtr = tempPtr;// currentPtr指向链表尾部 
									}
									else { //如果头结点紧接着空指针 
										headPtr->nextPtr = tempPtr;
										currentPtr = tempPtr;
									}
								}//else
							}//if
						}//else
						reqs[count].reqAct = EMPTY;
						is_overlap = 0;
					}
					else
						printf("Error when asking for space.\n"); //提示错误 
				}
				count++;//继续读取请求数组下一个请求
			}
			//加入内部优先
			break;
		
		case (2) ://顺便服务策略
			while (count<reqsSize){//还未将请求数组中所有的请求放入链表中，继续循环
				if (reqs[count].reqAct != -1){//当请求不为空时
					tempPtr = (LISTNODEPTR)malloc(sizeof(LISTNODE)); //分配结点内存
					if (tempPtr != NULL){ //插入结点
						tempPtr->reqData = reqs[count];//把第i个请求赋给tempPtr指向的结构 
						if (reqs[count].reqAct == OPEN || reqs[count].reqAct == CLOSE){//如果是开门关门指令 
							if (reqs[count].reqAct == OPEN)//开门请求
								doorReqFunc(1, elevatorPtr);
							else//关门请求
								doorReqFunc(2, elevatorPtr);
						}//OPEN　& CLOSE
						else{//如果不是开关门请求
								//查看是否有与现有请求重复的节点 
								LISTNODEPTR lastPtr = headPtr;//初始化
								LISTNODEPTR nowPtr = headPtr->nextPtr;
								while (nowPtr != NULL){
									if (nowPtr->reqData.reqAct == tempPtr->reqData.reqAct
										&& nowPtr->reqData.reqF == tempPtr->reqData.reqF){//如果有重复请求 
										is_overlap = 1;//代表有重复请求出现
										LISTNODEPTR T = nowPtr;//暂存重复请求的节点 
										lastPtr->nextPtr = T->nextPtr;
										free(T);//释放重复请求的节点 
										break;
									}
									else{
										lastPtr = nowPtr;//指针后移 
										nowPtr = nowPtr->nextPtr;
									}
								}
								if (is_overlap == 0){//如果不重复
									if ((*elevatorPtr).eleState == UP){ //在电梯上升时，如果存在一个低于当前目标楼层的向上或内部请求加入时，自动将目标楼层与该请求切换
										if (((tempPtr->reqData).reqAct == OUTSIDEUP || (tempPtr->reqData).reqAct == INSIDE) &&
											(*elevatorPtr).goalF > (tempPtr->reqData).reqF && (*elevatorPtr).nowF < (tempPtr->reqData).reqF){//判断条件
											//将目标楼层等效为一个请求结构，然后将此请求与新加入的请求相替换
											reqTemp.reqF = (*elevatorPtr).goalF;
											(*elevatorPtr).goalF = (tempPtr->reqData).reqF;
											(tempPtr->reqData).reqF = reqTemp.reqF;
											(*reqTempPtr).reqF = (*elevatorPtr).goalF;
											reqTemp.reqAct = (*reqTempPtr).reqAct;
											(*reqTempPtr).reqAct = (tempPtr->reqData).reqAct;
											(tempPtr->reqData).reqAct = reqTemp.reqAct;
										}
									}

									else if ((*elevatorPtr).eleState == DOWN){//在电梯下降时，如果存在一个高于当前目标楼层的向下或内部请求加入时，自动将目标楼层与该请求切换 
										if (((tempPtr->reqData).reqAct == OUTSIDEDOWN || (tempPtr->reqData).reqAct == INSIDE) &&
											(*elevatorPtr).goalF < (tempPtr->reqData).reqF && (*elevatorPtr).nowF >(tempPtr->reqData).reqF){//判断条件
											//将目标楼层等效为一个请求结构，然后将此请求与新加入的请求相替换
											reqTemp.reqF = (*elevatorPtr).goalF;
											(*elevatorPtr).goalF = (tempPtr->reqData).reqF;
											(tempPtr->reqData).reqF = reqTemp.reqF;
											(*reqTempPtr).reqF = (*elevatorPtr).goalF;
											reqTemp.reqAct = (*reqTempPtr).reqAct;
											(*reqTempPtr).reqAct = (tempPtr->reqData).reqAct;
											(tempPtr->reqData).reqAct = reqTemp.reqAct;
										}
									}

									tempPtr->nextPtr = NULL;//其他情况时直接将请求放至链表尾部，由sortServList进行处理
									if (currentPtr != NULL){
										while ((currentPtr->nextPtr) != NULL){//将指针移到链表尾部 
											currentPtr = currentPtr->nextPtr;
										}
										currentPtr->nextPtr = tempPtr;//将请求连到链表尾部 
										currentPtr = tempPtr;// currentPtr指向链表尾部 
									}
									else { //如果头结点紧接着空指针 
										headPtr->nextPtr = tempPtr;
										currentPtr = tempPtr;
									}
								}//判断重复的部分结束
							}//主要部分结束
						reqs[count].reqAct = EMPTY;//将此请求从reqs中清除
						is_overlap = 0;
					}
					else
						printf("Error when asking for space.\n"); //提示错误 
				}///判断请求不为空的部分结束
				count++;
			}//end of while
			break;
		case(3) ://宏福电梯策略
			while (count<reqsSize){
				if (reqs[count].reqAct != -1){//当请求不为空时
					tempPtr = (LISTNODEPTR)malloc(sizeof(LISTNODE)); //分配结点内存
					if (tempPtr != NULL){ //插入结点
						tempPtr->reqData = reqs[count];//把第i个请求赋给tempPtr指向的结构 
						if (reqs[count].reqAct == OPEN || reqs[count].reqAct == CLOSE){//如果是开门关门指令 
							if (reqs[count].reqAct == OPEN)//开门请求
								doorReqFunc(1, elevatorPtr);
							else//关门请求
								doorReqFunc(2, elevatorPtr);
						}//OPEN　& CLOSE
						else if (reqs[count].reqF<2 || reqs[count].reqF>5){//宏福策略重要点，故障电梯模式
								//如果不是开关门请求
								//查看是否有与现有请求重复的节点 
								LISTNODEPTR lastPtr = headPtr;//初始化
								LISTNODEPTR nowPtr = headPtr->nextPtr;
								while (nowPtr != NULL){
									if (nowPtr->reqData.reqAct == tempPtr->reqData.reqAct
										&& nowPtr->reqData.reqF == tempPtr->reqData.reqF){//如果有重复请求 
										is_overlap = 1;//代表有重复节点
										LISTNODEPTR T = nowPtr;//暂存重复请求的节点 
										lastPtr->nextPtr = T->nextPtr;
										free(T);//释放重复请求的节点 
										break;
									}
									else{
										lastPtr = nowPtr;//指针后移 
										nowPtr = nowPtr->nextPtr;
									}
								}
								if (is_overlap == 0){//如果不重复
									if ((*elevatorPtr).eleState == UP){ //在电梯上升时，如果存在一个低于当前目标楼层的向上或内部请求加入时，自动将目标楼层与该请求切换
										if (((tempPtr->reqData).reqAct == OUTSIDEUP || (tempPtr->reqData).reqAct == INSIDE) &&
											(*elevatorPtr).goalF > (tempPtr->reqData).reqF && (*elevatorPtr).nowF < (tempPtr->reqData).reqF){//判断条件
											reqTemp.reqF = (*elevatorPtr).goalF;//将目标楼层等效为一个请求结构，然后将此请求与新加入的请求相替换
											(*elevatorPtr).goalF = (tempPtr->reqData).reqF;
											(tempPtr->reqData).reqF = reqTemp.reqF;
											(*reqTempPtr).reqF = (*elevatorPtr).goalF;
											reqTemp.reqAct = (*reqTempPtr).reqAct;
											(*reqTempPtr).reqAct = (tempPtr->reqData).reqAct;
											(tempPtr->reqData).reqAct = reqTemp.reqAct;
										}
									}

									else if ((*elevatorPtr).eleState == DOWN){//在电梯下降时，如果存在一个高于当前目标楼层的向下或内部请求加入时，自动将目标楼层与该请求切换 
										if (((tempPtr->reqData).reqAct == OUTSIDEDOWN || (tempPtr->reqData).reqAct == INSIDE) &&
											(*elevatorPtr).goalF < (tempPtr->reqData).reqF && (*elevatorPtr).nowF >(tempPtr->reqData).reqF){//判断条件
											reqTemp.reqF = (*elevatorPtr).goalF;//将目标楼层等效为一个请求结构，然后将此请求与新加入的请求相替换
											(*elevatorPtr).goalF = (tempPtr->reqData).reqF;
											(tempPtr->reqData).reqF = reqTemp.reqF;
											(*reqTempPtr).reqF = (*elevatorPtr).goalF;
											reqTemp.reqAct = (*reqTempPtr).reqAct;
											(*reqTempPtr).reqAct = (tempPtr->reqData).reqAct;
											(tempPtr->reqData).reqAct = reqTemp.reqAct;
										}
									}

									tempPtr->nextPtr = NULL;//其他情况时直接将请求放至链表尾部，由sortServList进行处理
									if (currentPtr != NULL){
										while ((currentPtr->nextPtr) != NULL){//将指针移到链表尾部 
											currentPtr = currentPtr->nextPtr;
										}
										currentPtr->nextPtr = tempPtr;//将请求连到链表尾部 
										currentPtr = tempPtr;// currentPtr指向链表尾部 
									}
									else { //如果头结点紧接着空指针 
										headPtr->nextPtr = tempPtr;
										currentPtr = tempPtr;
									}
								}//判断重复的部分结束
							}//主要部分结束
						reqs[count].reqAct = EMPTY;//将此请求从reqs中清除
						is_overlap = 0;
					}
					else
						printf("Error when asking for space.\n"); //提示错误 
				}///判断请求不为空的部分结束
				count++;
			}//end of while
	}//end of switch 
}


//为顺便服务而设置的策略进程，当到达一个目标楼层时此进程会运行并确定下一个目标楼层，当确定目标楼层时将此请求提至链表首位
DWORD WINAPI addServListProc (LPVOID lpParameter){
	//定义并初始化变量
	struct HANDLETRANS * handleTransPtr = (struct HANDLETRANS *) lpParameter; //将传入的地址结构变量格式化
		int * serviceMethodPtr =  (* handleTransPtr).serviceMethodPtr;//定义策略模式变量
		LISTNODEPTR headp = (* handleTransPtr).headp;//定义请求链表头结点
		struct ELEVATOR * elevatorPtr= (* handleTransPtr).elevatorPtr; //定义电梯状态的结构变量地址
		int res = 0;//判断是否进行过顺便服务排序和内部请求清理 
	
	while (1){//循环运行
		if ((*elevatorPtr).eleState == UP || (*elevatorPtr).eleState == DOWN){//当电梯状态为上升或下降且到达终点时
			if ((*elevatorPtr).stateTime != 0 && res == 1){//如果进行过操作且stateTime!=0 
				res = 0;
			}
			if (res == 0 && (*elevatorPtr).nowF == (*elevatorPtr).goalF && (*elevatorPtr).stateTime == 0){
				deleteInsideReq(headp, elevatorPtr);
				if ((*serviceMethodPtr) == 2 || (*serviceMethodPtr) == 3){//仅在顺便服务时执行
					if ((*elevatorPtr).goalF == 9){//当到达最顶层或最低层时，将状态反转
						(*elevatorPtr).eleState = DOWN;
					}
					else if ((*elevatorPtr).goalF == 1){
						(*elevatorPtr).eleState = UP;
					}
					WaitForSingleObject(hMutex, INFINITE);//申请互斥变量
					deleteInsideReq(headp, elevatorPtr);//判断是否有内部请求需要删除，若有则删去
					sortServList(headp, elevatorPtr);//将链表中符合策略的一项提至最前
					res = 1;
					ReleaseMutex(hMutex); //释放互斥变量
				}//end of 顺便服务
			}
		}
		Sleep(500);//每0.5秒检查一次
	}//end of while(1)
}

void sortServList(LISTNODEPTR headp,struct ELEVATOR * elevatorPtr){
	//定义并初始化变量
	LISTNODEPTR currentPtr = headp->nextPtr, tempPtr = NULL; //初始化链表
	LISTNODEPTR lastPtr=headp,sortLastPtr=headp,sortCurrentPtr=headp;//初始化排序用的链表节点
	struct REQ reqTemp;//临时请求
	reqTemp.reqF=(*elevatorPtr).nowF;//初始化
	int sortRes=0;//是否排过序
	//进行筛选
	if (currentPtr!=NULL && currentPtr->nextPtr!=NULL){//当链表不为空时
		if ((*elevatorPtr).eleState==UP){//如果电梯此刻为向上
			reqTemp.reqAct=OUTSIDEUP;//临时请求的状态为向上
			for (reqTemp.reqF = 9; currentPtr!=NULL; lastPtr=currentPtr,currentPtr=currentPtr->nextPtr){
				//如果比当前层高的楼层有向上呼叫或者目标，那么以最低的高于当前楼层的有向上呼叫或者目标的楼层为目标
				if ((currentPtr->reqData).reqAct==OUTSIDEUP || (currentPtr->reqData).reqAct==INSIDE){
					if ((currentPtr->reqData).reqF>(*elevatorPtr).nowF && (currentPtr->reqData).reqF<reqTemp.reqF){
						//记录目前最优目标楼层的节点地址
						sortLastPtr=lastPtr;
						sortCurrentPtr=currentPtr;
						sortRes=1;
						reqTemp.reqF=(currentPtr->reqData).reqF;//储存暂时最优目标楼层，用于下次循环比较
					}//end of if
				}//end of if
			}
			if(sortRes!=1)
				for (reqTemp.reqF = 1, currentPtr = headp->nextPtr, lastPtr = headp;
					currentPtr != NULL; lastPtr = currentPtr, currentPtr = currentPtr->nextPtr){
					//如果无法确定目标楼层，那么以最高的向下呼叫或者目标所在楼层为电梯当前目标楼层
					if ((currentPtr->reqData).reqAct==OUTSIDEDOWN || (currentPtr->reqData).reqAct==INSIDE){
						if ((currentPtr->reqData).reqF>reqTemp.reqF){
							//记录目前最优目标楼层的节点地址
							sortLastPtr=lastPtr;
							sortCurrentPtr=currentPtr;
							sortRes=1;
							reqTemp.reqF=(currentPtr->reqData).reqF;//储存暂时最优目标楼层，用于下次循环比较
						}
					}
				}//end of for
			if(sortRes!=1)
				for (reqTemp.reqF = (*elevatorPtr).nowF, currentPtr = headp->nextPtr, lastPtr = headp;
					currentPtr != NULL; lastPtr = currentPtr, currentPtr = currentPtr->nextPtr){
					//如果无法确定目标楼层，那么以最低的向上呼叫所在楼层为电梯当前的目标楼层。
					if ((currentPtr->reqData).reqAct==OUTSIDEUP){
						if ((currentPtr->reqData).reqF<reqTemp.reqF){
							//记录目前最优目标楼层的节点地址
							sortLastPtr=lastPtr;
							sortCurrentPtr=currentPtr;
							sortRes=1;
							reqTemp.reqF=(currentPtr->reqData).reqF;//储存暂时最优目标楼层，用于下次循环比较
						}
					}
				}//end of for
		}///end of if(up)
		if ((*elevatorPtr).eleState==DOWN){//如果电梯此刻为向下
			reqTemp.reqAct=OUTSIDEDOWN;//临时请求的状态为向下
			for (reqTemp.reqF = 1, currentPtr = headp->nextPtr, lastPtr = headp; currentPtr != NULL;
				lastPtr = currentPtr, currentPtr = currentPtr->nextPtr){
				//如果比当前层低的楼层有向下呼叫或者目标，那么以最高的低于当前楼层的有向下呼叫或者目标的楼层为目标
				if ((currentPtr->reqData).reqAct==OUTSIDEDOWN || (currentPtr->reqData).reqAct==INSIDE){
					if ((currentPtr->reqData).reqF<(*elevatorPtr).nowF && (currentPtr->reqData).reqF>reqTemp.reqF){
						//记录目前最优目标楼层的节点地址
						sortLastPtr=lastPtr;
						sortCurrentPtr=currentPtr;
						sortRes=1;
						reqTemp.reqF=(currentPtr->reqData).reqF;//储存暂时最优目标楼层，用于下次循环比较
					}
				}
			}//end of for
			if(sortRes!=1)
			for (reqTemp.reqF = 9, currentPtr = headp->nextPtr, lastPtr = headp; currentPtr != NULL;
					lastPtr = currentPtr, currentPtr = currentPtr->nextPtr){
					//如果无法确定目标楼层，那么以最低的向上呼叫或者目标所在楼层为电梯当前目标楼层
					if ((currentPtr->reqData).reqAct==OUTSIDEUP || (currentPtr->reqData).reqAct==INSIDE){
						if ((currentPtr->reqData).reqF<reqTemp.reqF){
							//记录目前最优目标楼层的节点地址
							sortLastPtr=lastPtr;
							sortCurrentPtr=currentPtr;
							sortRes=1;
							reqTemp.reqF=(currentPtr->reqData).reqF;//储存暂时最优目标楼层，用于下次循环比较
						}
					}
				}//end of for 
			if(sortRes!=1)
				for (reqTemp.reqF = (*elevatorPtr).nowF, currentPtr = headp->nextPtr, lastPtr = headp;
					currentPtr != NULL; lastPtr = currentPtr, currentPtr = currentPtr->nextPtr){
					//如果无法确定目标楼层，那么以最高的向下呼叫楼层为目标楼层
					if ((currentPtr->reqData).reqAct=OUTSIDEDOWN){
						if ((currentPtr->reqData).reqF>reqTemp.reqF){
							//记录目前最优目标楼层的节点地址
							sortLastPtr=lastPtr;
							sortCurrentPtr=currentPtr;
							sortRes=1;
							reqTemp.reqF=(currentPtr->reqData).reqF;//储存暂时最优目标楼层，用于下次循环比较
						}
					}
				}//end of for
		}//end of if(down)
		if(sortRes==1){//当要进行提前时，完成剩余的节点交换操作
			tempPtr = headp->nextPtr;
			headp->nextPtr = sortCurrentPtr;
			sortLastPtr->nextPtr = tempPtr;
			tempPtr = (sortLastPtr->nextPtr)->nextPtr;
			(sortLastPtr->nextPtr)->nextPtr = sortCurrentPtr->nextPtr;
			sortCurrentPtr->nextPtr = tempPtr;
		}
	}
}

//删除内部请求节点
void deleteInsideReq(LISTNODEPTR headPtr, struct ELEVATOR * elevatorPtr){
	LISTNODEPTR currentPtr = headPtr->nextPtr, lastPtr = headPtr;//初始化
	while (currentPtr != NULL){
		if ((currentPtr->reqData).reqAct == INSIDE && (currentPtr->reqData).reqF == (*elevatorPtr).nowF){
			//电梯到达某楼层，此楼层有内部请求，则删去内部请求
			lastPtr->nextPtr = currentPtr->nextPtr;
			free(currentPtr);
			currentPtr = lastPtr->nextPtr;
		}
		else
			currentPtr = currentPtr->nextPtr;//移到下个节点继续搜寻
	}
}

//end of strategy.c
