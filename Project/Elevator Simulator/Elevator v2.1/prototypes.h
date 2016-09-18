//output.c
int openFileToWrite(char inFileName[] ,FILE * * fpout);
//根据文件名打开输入输出文件 ,文件结构变量由fpout指向。打开成功，返回1，否则返回0

void set_fileOutput(FILE *fpout, int timeCount, struct ELEVATOR elevator, LISTNODEPTR headp, int detail);
//将电梯状态等信息输出至文件中

//state.c
void runService(struct ELEVATOR * elePtr, LISTNODEPTR headp, struct REQ * reqTempPtr);
//引入请求链表计算电梯状态，并将当前相应存入reqTemp中
void deleteListNode (LISTNODEPTR listPtr);
//删除节点

void initElevator(struct ELEVATOR * elePtr);
//对电梯状态进行初始化

void doorReqFunc(int req, struct ELEVATOR * elevatorPtr);

//graphic.c
DWORD WINAPI graphicOutProc(LPVOID lpParameter);
//动画输出进程

//strategy.c
void addServList(int reqMode, LISTNODEPTR headPtr, struct REQ reqs[], int reqsSize,struct ELEVATOR * elevatorPtr, struct REQ * reqTempPtr); 
//将请求根据策略不同，插入请求链表的合适位置

DWORD WINAPI addServListProc(LPVOID lpParameter);
//策略进程，为顺便服务进行添加

void sortServList(LISTNODEPTR headPtr, struct ELEVATOR * elevatorPtr);
//顺便服务的策略函数，将请求按照顺便服务策略进行实时添加

void deleteInsideReq(LISTNODEPTR headPtr, struct ELEVATOR * elevatorPtr);

//input.c
DWORD WINAPI keyboardInputProc(LPVOID lpParameter);
//键盘输入进程，调用keyboardinput函数后调用addServList函数将reqs[]的请求添加入链表中

void keyboardInput(struct REQ reqs[], int * serviceMethodPtr);
//键盘输入函数，将输入的请求存储入req[]数组中

void mouseInput(struct REQ reqs[], int * serviceMethodPtr);
//鼠标输入函数，将鼠标输入转换成相应的请求

DWORD WINAPI mouseInputProc(LPVOID lpParameter);
//鼠标输入进程，调用mouseInput函数和addServList函数
