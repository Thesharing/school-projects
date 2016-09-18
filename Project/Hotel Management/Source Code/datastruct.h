#ifndef DATASTRUCT
#define DATASTRUCT

#include <QString>
#include <QDateTime>

struct roomInfo;
struct guestInfo;
struct roomNode;
struct guestNode;

enum roomStatus{
    empty,checked,booked,checkandbook
}; //对应 空房 已入住 已预定 入住且预定

enum roomType{
    standard,doublebed,largebed,business,vip
};//对应 标准间 双人间 大床间 商务间 贵宾间

//以下是1.0版本时附带的链表的数据结构，为创建数据库方便没有删除
/*
struct roomInfo{ //房间信息
    int number;//房间号
    enum roomType type; //房间类型
    enum roomStatus status; //房间状态
    struct datetime checkInTime; //入住时间
    struct datetime checkOutTime; //离店时间
    struct guestNode * person; //旅客信息
};

struct guestInfo{ //宾客信息
    QString name; //姓名
    QString id; //身份证号
    bool gender; //性别
    QString workplace;//工作单位 ***
    QString phoneNum; //电话号码
    int consume;//房间内消费 ***
    int age;
    struct roomNode * room; //对应的房间信息
};

struct roomNode{ //房间表的节点
    struct roomNode * prev;
    struct roomInfo data; //房间信息
    struct roomNode * next;
};

struct bookNode{ //预定表的节点
    struct bookNode * prev;
    QString name; //预定信息
    QString phoneNum; //手机号
    struct datetime checkInTime; //预计入住时间
    struct datetime checkOutTime; //预计离店时间
    struct roomNode * roomNodePtr; //指向房间表节点的地址
    struct bookNode * next;
};

struct guestNode{ //宾客信息表的节点
    struct guestNode * prev;
    struct guestInfo data; //宾客信息
    struct guestNode * next;
};
*/

#endif // DATASTRUCT

