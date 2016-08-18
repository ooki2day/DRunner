#ifndef SERVER_H
#define SERVER_H

#define PORT 12345

#include <QTcpServer>
#include <QTcpSocket>

#include "qt-solutions/qtservice/src/qtservice.h"

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QObject *parent = 0);
    void incomingConnection(int socket);

signals:
    void recvData(QString str);

private slots:
    void recvCommandFromSocket();
    void closeSocket();
};

#endif // SERVER_H
