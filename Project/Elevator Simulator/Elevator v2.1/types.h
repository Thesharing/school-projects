//types.h 定义变量与常量


//定义常用的时间常量
#define ONELAYERLEN 3  //电梯运行一层需要的时间片
#define STOPLEN 5    //电梯停靠的时间片
#define OPENLEN 3    //电梯开门需要的时间片
#define CLOSELEN 3   //电梯关门需要的时间片


//枚举常量
enum STATE {IDLE,UP,DOWN,DOOROPENING,DOORCLOSING,STOP};
//枚举电梯状态(空闲，上行，下行，开门，关门，停靠)
enum ACTION {EMPTY=-1, INSIDE,OUTSIDEUP,OUTSIDEDOWN,OPEN,CLOSE};
//枚举电梯动作请求(空闲，内部请求，上升，下降，开门，关门) 


//定义结构
struct ELEVATOR {
	enum STATE eleState;
	int nowF;
	int goalF;
	int stopTime;
	int stateTime;
};//电梯结构，记录电梯状态、所在楼层、停靠时长、时间片等
struct REQ{
	enum ACTION reqAct;
	int reqF;
};//请求事件
struct LIST{
	struct REQ reqData;
	struct LIST * nextPtr;
};//请求队列链表节点

typedef struct LIST LISTNODE;
typedef LISTNODE * LISTNODEPTR;//对LIST结构进行简化

struct HANDLETRANS{
	LISTNODEPTR headp;//请求链表 
	struct ELEVATOR * elevatorPtr; //电梯状态 
	int * serviceMethodPtr; //服务模式的地址 
	int * reqCountPtr; //请求数的地址 
	int * reqTimePtr; //请求时间的地址
	struct REQ * reqTempPtr;
};//将相关数据的地址传入进程中