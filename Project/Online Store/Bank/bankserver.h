#ifndef BANKSERVER_H
#define BANKSERVER_H

#include <QtNetwork>

/* bankserver.h定义了TCP的服务器线程bankServer*
 * 通过继承QTcpServer，在有客户端要建立连接时，
 * 建立TcpSocket线程与客户端建立TCP连接 */

class bankServer : public QTcpServer
{
    Q_OBJECT
public:
    bankServer(QObject *parent=0);
    QTcpSocket * clientConnection;

private slots:
    void createConnection();
    void readClient();
};

#endif // BANKSERVER_H
