#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "graphics.h"
#include "types.h"
#include "prototypes.h"

extern HANDLE hMutex;

//graphic.c

DWORD WINAPI graphicOutProc(LPVOID lpParameter){
	//定义并初始化变量
	struct HANDLETRANS * handleTransPtr = (struct HANDLETRANS *) lpParameter; //将传入的链表格式化 
	LISTNODEPTR headPtr = (*handleTransPtr).headp;//请求链表
	struct ELEVATOR elevator =*( (*handleTransPtr).elevatorPtr);//电梯状态
	int serviceMethod = *((*handleTransPtr).serviceMethodPtr);//策略模式
	struct REQ reqTemp = *((*handleTransPtr).reqTempPtr);//临时请求
	float fpscount=0;//帧数计数变量
	int fpsclear = 0;//是否将fpscount清零
	int insideReq[9] = { 0 };//内部请求数组
	int outsideUpReq[9] = { 0 };//外部向上请求数组
	int outsideDownReq[9] = { 0 };//外部向下请求数组
	LISTNODEPTR currentPtr = headPtr;//对请求链表进行读取，将相应的请求显示出来
	struct ELEVATOR lastState=elevator;//前一状态
	
	//图片的文件名字符串
	char nameInOn[] = "pic\\button-1-on.jpg";
	char nameInOff[] = "pic\\button-2-off.jpg";
	char nameInOri[] = "pic\\button-1-ori.jpg";
	char nameFloor[] = "pic\\elevator-1.jpg";
	char * nameOut[6] = { "pic\\button-up-ori.jpg", "pic\\button-up-on.jpg", "pic\\button-up-off.jpg",
		"pic\\button-down-ori.jpg", "pic\\button-down-on.jpg", "pic\\button-down.jpg" };
	char nameNowFloor[] = "pic\\floor-1.bmp";

	//图片的坐标
	int i;//临时变量
	int buttonInX = 576, buttonInY = 192;//内部请求
	int buttonOutUpX = 192, buttonOutUpY = 96;//外部向上
	int buttonOutDownX = 240, buttonOutDownY = 48;//外部向下
	int buttonFloorX = 96, buttonFloorY = 48;//楼层数字
	int elevatorX = 96, elevatorY = 96;//电梯
	int serviceX = 336, serviceY = 288;//策略服务
	int nowFloorX = 624, nowFloorY = 144;//当前楼层
	int goalFloorX = 480, goalFloorY = 144;//目标楼层
	int buttonDoorX = 624, buttonDoorY = 192;//开关门
	int doorX, doorY;//门
	int doorCoverX=elevatorX+48, doorCoverY;//门的挡板
	int directionX = 432, directionY = 144;
	setinitmode(INIT_NOBORDER);
	initgraph(768, 672);//创建图形环境

	//获取图片资源
	PIMAGE imgBackground = newimage();//背景图片
	getimage(imgBackground, "pic//background.jpg");
	
	PIMAGE imgFloor[9];//楼层
	for (i = 0; i < 9; i++){
		imgFloor[i] = newimage();
		nameFloor[13] = 8 - i + 49;
		getimage(imgFloor[i], nameFloor);
	}
	PIMAGE buttonInOri[9];//内部不亮
	for (i = 0; i < 9; i++){
		nameInOri[11] = i + 49;
		buttonInOri[i] = newimage();
		getimage(buttonInOri[i], nameInOri);
	}
	PIMAGE buttonInOn[9];//内部亮
	for (i = 0; i < 9; i++){
		nameInOn[11] = i + 49;
		buttonInOn[i] = newimage();
		getimage(buttonInOn[i], nameInOn);
	}
	PIMAGE buttonOutUpOn = newimage();//外部向上亮
	getimage(buttonOutUpOn, nameOut[1]);
	PIMAGE buttonOutUpOri = newimage();//外部向上不亮
	getimage(buttonOutUpOri, nameOut[0]);
	PIMAGE buttonOutDownOn = newimage();//外部向下亮
	getimage(buttonOutDownOn, nameOut[4]);
	PIMAGE buttonOutDownOri = newimage();//外部向下不亮
	getimage(buttonOutDownOri, nameOut[3]);
	PIMAGE imgElevator = newimage();//电梯
	getimage(imgElevator, "pic\\elevator-inside.jpg");
	PIMAGE imgNowFloor[9];//当前楼层
	for (i = 0; i < 9; i++){
		imgNowFloor[i] = newimage();
		nameNowFloor[10] = i + 49;
		getimage(imgNowFloor[i], nameNowFloor);
	}
	PIMAGE imgService1off = newimage();//策略模式
	getimage(imgService1off, "pic\\service-1-of.jpg");
	PIMAGE imgService1on = newimage();
	getimage(imgService1on, "pic\\service-1-on.jpg");
	PIMAGE imgService2off = newimage();
	getimage(imgService2off, "pic\\service-2-of.jpg");
	PIMAGE imgService2on = newimage();
	getimage(imgService2on, "pic\\service-2-on.jpg");
	PIMAGE imgService3on = newimage();
	getimage(imgService3on, "pic\\service-3-on.jpg");
	PIMAGE imgService3off = newimage();
	getimage(imgService3off, "pic\\service-3-off.jpg");
	PIMAGE imgButtonDoorOpen = newimage(); //开门键
	getimage(imgButtonDoorOpen, "pic\\door-open.jpg");
	PIMAGE imgButtonDoorClose = newimage();//关门键
	getimage(imgButtonDoorClose, "pic\\door-close.jpg");
	PIMAGE imgDoor = newimage();//门
	getimage(imgDoor, "pic\\elevator-door.jpg");
	PIMAGE imgDoorCoverRight = newimage();//右门挡
	getimage(imgDoorCoverRight, "pic\\elevator-doorcover-right.jpg");
	PIMAGE imgDoorCoverLeft = newimage();//左门挡
	getimage(imgDoorCoverLeft, "pic\\elevator-doorcover-left.jpg");
	PIMAGE imgDirectionUp = newimage();
	getimage(imgDirectionUp, "pic\\up.jpg");
	PIMAGE imgDirectionDown = newimage();
	getimage(imgDirectionDown, "pic\\down.jpg");
	PIMAGE imgDirectionOther = newimage();
	getimage(imgDirectionOther, "pic\\other.jpg");
	PIMAGE imgButtonOutDownOff = newimage();
	getimage(imgButtonOutDownOff, nameOut[5]);
	PIMAGE buttonOutUpOff = newimage();
	getimage(buttonOutUpOff, nameOut[2]);
	PIMAGE buttonNoUse = newimage();
	getimage(buttonNoUse, nameOut[2]);
	PIMAGE buttonInOff[4];//内部禁用
	for (i = 0; i < 9; i++){
		nameInOff[11] = i + 50;
		buttonInOff[i] = newimage();
		getimage(buttonInOff[i], nameInOff);
	}

	while (1){///开始图形化输出的循环
		headPtr = (*handleTransPtr).headp;//获取请求链表
		elevator = *((*handleTransPtr).elevatorPtr);//获取电梯状态
		serviceMethod = *((*handleTransPtr).serviceMethodPtr);//获取策略模式
		reqTemp.reqAct = (*((*handleTransPtr).reqTempPtr)).reqAct;//获取临时请求的请求
		reqTemp.reqF = (*((*handleTransPtr).reqTempPtr)).reqF;//获取临时请求的楼层
		currentPtr = headPtr;//将请求链表的首节点挂接在当前节点上
		
		//根据上一状态和当前状态来判断是否将fpscount清零
		if (lastState.eleState == STOP){
			if (elevator.eleState != STOP){
				fpscount = 0;
			}
		}
		else if (lastState.eleState == UP || lastState.eleState == DOWN){
			if (elevator.eleState == DOOROPENING || lastState.nowF!=elevator.nowF || elevator.nowF==elevator.goalF){
				fpscount = 0;
			}
		}
		else if (elevator.eleState == DOOROPENING){//
			if (elevator.stateTime == 3){
				if (fpsclear == 0){
					fpscount = 0;
					fpsclear = 1;
				}
			}
			else fpsclear = 0;
		}
		else if (lastState.eleState == DOORCLOSING){
			if (elevator.stateTime == 3){
				if (fpsclear == 0){
					fpscount = 0;
					fpsclear = 1;
				}
			}
			else fpsclear = 0;
		}
		else if (lastState.eleState == IDLE && elevator.eleState != IDLE){
			fpscount = 0;
		}
		lastState = elevator;
		
		//将内外相应存入相应的数组中
		while ((currentPtr->nextPtr) != NULL){
			currentPtr = currentPtr->nextPtr; //从头开始进行查询，每一次都进行查询 
			if (currentPtr!=NULL)
			switch (currentPtr->reqData.reqAct){ //对于链表内的内部和外部请求 
				case (INSIDE) :
					insideReq[currentPtr->reqData.reqF - 1] = 1;
					break;
				case (OUTSIDEUP) :
					outsideUpReq[currentPtr->reqData.reqF - 1] = 1;
					break;
				case (OUTSIDEDOWN) :
					outsideDownReq[currentPtr->reqData.reqF - 1] = 1;
			}
		}
		switch (reqTemp.reqAct){ //将已经被读取而正在相应的请求存入数组中
			case (INSIDE) :
				insideReq[reqTemp.reqF - 1] = 1;
				break;
			case (OUTSIDEUP) :
				outsideUpReq[reqTemp.reqF - 1] = 1;
				break;
			case (OUTSIDEDOWN) :
				outsideDownReq[reqTemp.reqF - 1] = 1;
		}

		cleardevice();//清屏
		
		//将相应图形输出
		putimage(0, 0, imgBackground);
		for (i = 0; i < 8; i++){//外部向上
			buttonOutUpY += 48;
			if (outsideUpReq[7-i] == 1){
				putimage(buttonOutUpX, buttonOutUpY, buttonOutUpOn);
			}
			else{
				putimage(buttonOutUpX, buttonOutUpY, buttonOutUpOri);
			}
			outsideUpReq[7-i] = 0;
		}
		buttonOutUpY = 96;
		for (i = 0; i < 8; i++){//外部向下
			buttonOutDownY += 48;
			if (outsideDownReq[8-i] == 1){
				putimage(buttonOutDownX, buttonOutDownY, buttonOutDownOn);
			}
			else{
				putimage(buttonOutDownX, buttonOutDownY, buttonOutDownOri);
			}
			outsideDownReq[8-i] = 0;
		}
		buttonOutDownY = 48;
		for (i = 8; i >= 0; i--){//内部请求
			buttonInY += 48;
			if (i == 4){
				buttonInX += 48;
				buttonInY = 336;
			}
			if (insideReq[i] == 0){
				putimage(buttonInX, buttonInY, buttonInOri[i]);
			}
			else{
				putimage(buttonInX, buttonInY, buttonInOn[i]);
			}
			insideReq[i] = 0;
		}
		buttonInX = 576;
		buttonInY = 192;
		for (i = 0; i < 9; i++){//楼层名称
			buttonFloorY += 48;
			putimage(buttonFloorX, buttonFloorY, imgFloor[i]);
		}
		buttonFloorY = 48;
		buttonDoorX -= 48;
		putimage(buttonDoorX, buttonDoorY, imgButtonDoorOpen);//开门按钮
		buttonDoorX += 48;//关门按钮
		putimage(buttonDoorX, buttonDoorY, imgButtonDoorClose);
		putimage(nowFloorX, nowFloorY, imgNowFloor[elevator.nowF - 1]);//当前楼层
		putimage(goalFloorX, goalFloorY, imgNowFloor[elevator.goalF - 1]);//目标楼层
		if (serviceMethod == 1){
			putimage(serviceX, serviceY, imgService1on);//策略显示
			putimage(serviceX, serviceY + 48, imgService2off);
			putimage(serviceX, serviceY + 96, imgService3off);
		}
		else if (serviceMethod == 2){
			putimage(serviceX, serviceY, imgService1off);
			putimage(serviceX, serviceY + 48, imgService2on);
			putimage(serviceX, serviceY + 96, imgService3off);
		}
		else{
			putimage(serviceX, serviceY, imgService1off);
			putimage(serviceX, serviceY + 48, imgService2off);
			putimage(serviceX, serviceY + 96, imgService3on);
		}
		if (elevator.eleState == UP){//电梯
			elevatorY = (9 - elevator.nowF) * 48 + 96 - (int)(fpscount / 63 * 60 / 1.25);
		}
		else if (elevator.eleState == DOWN){
			elevatorY = (9 - elevator.nowF) * 48 + 96 + (int)(fpscount / 63 * 60 / 1.25);
		}
		else {
			elevatorY = 96+ (9 - elevator.nowF) * 48;
		}
		putimage(elevatorX, elevatorY, imgElevator);
		doorY = elevatorY;//电梯左门
		if (elevator.eleState == DOOROPENING){
			doorX = elevatorX - (int)(fpscount / 66 * 60 / 2.5);
		}
		else if (elevator.eleState == DOORCLOSING){
			doorX = elevatorX - 24 + (int)(fpscount / 66 * 60 / 2.5);
		}
		else if (elevator.eleState == STOP){
			doorX = elevatorX - 24;
		}
		else{
			doorX = elevatorX;
		}
		putimage(doorX, doorY, imgDoor);
		if (elevator.eleState == DOOROPENING){//电梯右门
			doorX = elevatorX + 24 + (int)(fpscount / 66 * 60 / 2.5);
		}
		else if (elevator.eleState == DOORCLOSING){
			doorX = elevatorX + 48 - (int)(fpscount / 66 * 60 / 2.5);
		}
		else if (elevator.eleState == STOP){
			doorX = elevatorX + 48;
		}
		else {
			doorX = elevatorX + 24;
		}
		putimage(doorX, doorY, imgDoor);
		doorCoverX -= 72;
		doorCoverY = elevatorY;//电梯左遮挡
		putimage(doorCoverX, doorCoverY, imgDoorCoverLeft);
		doorCoverX += 72;//电梯右遮挡
		putimage(doorCoverX, doorCoverY, imgDoorCoverRight);
		if (elevator.eleState == UP){
			putimage(directionX, directionY, imgDirectionUp);
		}
		else if (elevator.eleState == DOWN){
			putimage(directionX, directionY, imgDirectionDown);
		}
		else {
			putimage(directionX, directionY, imgDirectionOther);
		}
		if (serviceMethod == 3){
			buttonOutUpY = 240;
			for (i = 0; i < 4; i++){//外部向上
				buttonOutUpY += 48;
				putimage(buttonOutUpX, buttonOutUpY, buttonOutUpOff);
			}
			buttonOutUpY = 96;
			buttonOutDownY = 240;
			for (i = 0; i < 4; i++){//外部向下
				buttonOutDownY += 48;
				putimage(buttonOutDownX, buttonOutDownY, imgButtonOutDownOff);
			}
			buttonOutDownY = 48;
			buttonInX = 624;
			buttonInY = 288;
			for (i = 3; i >= 0; i--){//内部请求
				buttonInY += 48;
				putimage(buttonInX, buttonInY, buttonInOff[i]);
			}
			buttonInX = 576;
			buttonInY = 192;
		}

		fpscount+=1;//帧数加一
		delay_ms(50);//每秒近似刷新20帧
	}//end of while
	
	//删除图片缓存
	for (i = 0; i < 9; i++)
		delimage(imgNowFloor[i]);
	for (i = 0; i < 9; i++)
		delimage(imgFloor[i]);
	for (i = 0; i < 9; i++)
		delimage(buttonInOri[i]);
	for (i = 0; i < 9; i++)
		delimage(buttonInOn[i]);
	delimage(imgService1off);
	delimage(imgService1on);
	delimage(imgService2off);
	delimage(imgService2on);
	delimage(imgBackground);
	delimage(imgElevator);
	delimage(imgDoor);
	delimage(imgDoorCoverLeft);
	delimage(imgDoorCoverRight);
	delimage(buttonOutUpOn);
	delimage(buttonOutUpOri);
	delimage(buttonOutDownOn);
	delimage(buttonOutDownOri);
	delimage(imgButtonDoorOpen);
	delimage(imgButtonDoorClose);
	delimage(imgButtonOutDownOff);
	closegraph();//关闭图形环境
	DWORD res = 1;//C++中定义的返回值
	return res;
}