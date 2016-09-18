#ifndef REQUESTLIST_H
#define REQUESTLIST_H

#include "request.h"
#include <QVector>

class RequestList
{
public:
    QVector<Request> * requestList;
    RequestList(){
        requestList = new QVector<Request>;
    }

    ~RequestList(){
        delete requestList;
    }
    void addSlave(int roomNum);
    Request* findRequest(int roomNum);
    //void deleteRequest(int roomNum);
    void updateRequest(Request newRequest);
    void setRequestDealt(int roomNum);
    void dealRequests();
    Request* findNextAvailableRequest();
    void setAllRequestDealt();
};

#endif // REQUESTLIST_H
