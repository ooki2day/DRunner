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
    void connectStateChanged(bool connected);
    void parsedDataReady(const Utils::SocketData &data);
    void socketDisconnected(qintptr descriptor);
    void recvDataFromSocket(qintptr descriptor, QHostAddress address, quint16 port, int dataSize);

public slots:
    void sendToSocket(qintptr descriptor, const QByteArray &data);

private slots:
    void closeSocket();
    void newConnection();
    void socketReadyRead();

private:
    QTcpServer *m_server;
    QHash<qintptr, QTcpSocket*> m_descriptorsHash;
    QHash<QTcpSocket*, qintptr> m_socketHash;
};

#endif // SERVER_H
