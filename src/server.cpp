#include "server.h"

Server::Server(QObject *parent) :
    QObject(parent)
{
    m_server = new QTcpServer(this);

    connect(m_server, &QTcpServer::newConnection,
            this, &Server::newConnection);
}

bool Server::isListening() {

    return m_server->isListening() ? true : m_server->listen(QHostAddress::Any, PORT);
}

void Server::newConnection() {

    auto socket = m_server->nextPendingConnection();

    connect(socket, &QTcpSocket::readyRead, this, &Server::socketReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::closeSocket);
}

void Server::socketReadyRead() {

    auto sock = (QTcpSocket*)sender();

    if(sock->canReadLine()) {

        auto data = sock->readLine();

        emit recvCommandFromSocket(data);
    }
}

void Server::closeSocket() {

    auto sock = (QTcpSocket*)sender();
    sock->deleteLater();
}

void Server::closeServer() {

    m_server->close();
}

Server::~Server() {

    closeServer();
}
