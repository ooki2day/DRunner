#include "server.h"

Server::Server(QObject *parent) :
    QObject(parent)
{
    m_server = new QTcpServer(this);
    m_server->isListening() ? true : m_server->listen(QHostAddress::Any, PORT);

    connect(m_server, &QTcpServer::newConnection,
            this, &Server::newConnection);

    qRegisterMetaType<Utils::SocketData>("Utils::SocketData");
    qRegisterMetaType<QHostAddress>("QHostAddress");
}

void Server::newConnection() {
    auto socket = m_server->nextPendingConnection();
    m_descriptorsHash[socket->socketDescriptor()] = socket;
    m_socketHash[socket] = socket->socketDescriptor();

    connect(socket, &QTcpSocket::readyRead, this, &Server::socketReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::closeSocket);
}

void Server::socketReadyRead() {
    auto sock = (QTcpSocket*)sender();
    if(!sock)
        return;

    if(sock->canReadLine()) {
        auto data = sock->readLine();

        Parser *parser = new Parser { sock->socketDescriptor(), data };
        connect(parser, &Parser::socketDataReady,
                this, &Server::parsedDataReady);

        emit recvDataFromSocket(sock->socketDescriptor(), sock->peerAddress(),
                                sock->peerPort(), data.size());
        QThreadPool::globalInstance()->start(parser);
    }
}

void Server::sendToSocket(qintptr descriptor, const QByteArray &data) {
    if(m_descriptorsHash.contains(descriptor)) {
        m_descriptorsHash[descriptor]->write(data);
    }
}

void Server::closeSocket() {
    auto sock = (QTcpSocket*)sender();

    if(m_socketHash.contains(sock)) {
        emit socketDisconnected(m_socketHash[sock]);
        m_socketHash.remove(sock);
    }
    if(m_descriptorsHash.contains(sock->socketDescriptor())) {
        m_descriptorsHash.remove(sock->socketDescriptor());
    }

    sock->deleteLater();
}

void Server::closeServer() {
    qDeleteAll(m_descriptorsHash);
    m_socketHash.clear();
    m_server->close();
}

Server::~Server() {
    closeServer();
}
