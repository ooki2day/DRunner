#include "server.h"

#include <QDebug>

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
    listen(QHostAddress::Any, PORT);
}

void Server::incomingConnection(int socket) {

    auto *sock = new QTcpSocket(this);
    connect(sock, &QTcpSocket::readyRead, this, &Server::recvCommandFromSocket);
    connect(sock, &QTcpSocket::disconnected, this, &Server::closeSocket);

    sock->setSocketDescriptor(socket);

//    QtServiceBase::instance()->logMessage("");
}

void Server::recvCommandFromSocket() {

    auto sock = (QTcpSocket*)sender();

    if(sock->canReadLine()) {

        auto data = sock->readLine();

        qDebug() << data;
    }
}

void Server::closeSocket() {

    auto sock = (QTcpSocket*)sender();
    sock->deleteLater();
}
