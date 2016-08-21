#ifndef SERVER_H
#define SERVER_H

#define PORT 12345

#include <QTcpServer>
#include <QTcpSocket>
#include <QThreadPool>

#include "qt-solutions/qtservice/src/qtservice.h"
#include "parser.h"
#include "processlauncher.h"

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
    void connectStateChanged(bool connected);

private slots:
    void closeSocket();
    void newConnection();
    void socketReadyRead();
    void getParsedData(const Utils::SocketData &data);

private:
    QTcpServer *m_server;
};

#endif // SERVER_H
