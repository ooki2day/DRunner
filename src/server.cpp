#include "server.h"

Server::Server(QObject *parent) :
    QObject(parent)
{
    m_server = new QTcpServer(this);
    m_server->isListening() ? true : m_server->listen(QHostAddress::Any, PORT);

    connect(m_server, &QTcpServer::newConnection,
            this, &Server::newConnection);

    qRegisterMetaType<Utils::SocketData>("Utils::SocketData");
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

        Parser *parser = new Parser { sock->socketDescriptor(), data };
        connect(parser, &Parser::socketDataReady,
                this, &Server::getParsedData);

        QThreadPool::globalInstance()->start(parser);
    }
}

void Server::getParsedData(const Utils::SocketData &data) {

    ProcessLauncher *launcher = new ProcessLauncher(this);
    launcher->startNewProcess(data);
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
