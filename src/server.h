#ifndef SERVER_H
#define SERVER_H

#define PORT 12345

#include <QTcpServer>
#include <QTcpSocket>

#include "qt-solutions/qtservice/src/qtservice.h"

class Server : public QObject
{
    Q_OBJECT

public:
    Server(QObject *parent = 0);
    ~Server();

    bool isListening();
    void closeServer();

signals:
    void recvCommandFromSocket(const QByteArray &command);

private slots:
    void closeSocket();
    void newConnection();
    void socketReadyRead();

private:
    QTcpServer *m_server;
};

#endif // SERVER_H
