#include "drunner.h"

DRunner::DRunner(int argc, char **argv) :
    QtService<QCoreApplication>(argc, argv, "DRunner")
{
    setServiceDescription("A dummy HTTP service implemented with Qt");
    setServiceFlags(QtServiceBase::CanBeSuspended);

    m_serverThread = new QThread();
}

bool DRunner::initLogFile() {

    m_logFile = new QFile(QDir::tempPath() + "/DRunner.log");
    if(m_logFile->open(QIODevice::WriteOnly)) {

        m_logFile->close();
        return true;
    }
    return false;
}

void DRunner::writeToLogFile(QString text) {

    if(m_logFile->open(QIODevice::Append | QIODevice::Text)) {

        m_logFile->write(text.toLocal8Bit());
        m_logFile->close();
    }

    logMessage(QObject::tr("Can't write to log file!"));
}

void DRunner::parseArgs(const QStringList &list) {

    QFile file;
    for(int i = 1; i < list.size(); ++i) {

        file.setFileName(list.at(i));
        if(file.exists()) {

            readAllowedProgramsFromFile(file);
            file.close();
            return;
        }
    }

    logMessage(QObject::tr("The file with allowed programs not exist."));
}

void DRunner::readAllowedProgramsFromFile(QFile &file) {


    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {

        while(!file.atEnd()) {

            m_allowedProgramsList.append(file.readLine());
        }
    } else {

        logMessage(QObject::tr("Can't open the file with allowed programs."));
    }
}

void DRunner::start() {

    auto *app = application();
    if(!initLogFile()) {

        logMessage(QObject::tr("Can't open the log file."));
        app->quit();
        return;
    }

    if(!serverInitCurrectly()) {

        logMessage(QObject::tr("Failed to start the server"), QtServiceBase::Error);
        app->quit();
        return;
    }

    parseArgs(QCoreApplication::arguments());
}

bool DRunner::serverInitCurrectly() {

    m_server = new Server();
    m_server->moveToThread(m_serverThread);
    m_serverThread->start();

    connect(m_server, &Server::recvCommandFromSocket,
            this, &DRunner::writeToLogFile);
    connect(m_serverThread, &QThread::finished,
            m_server, &Server::deleteLater);
    connect(m_server, &Server::connectStateChanged,
            this, &DRunner::serverStateChanged);

    return true;
}

void DRunner::serverStateChanged(bool connected) {

    if(!connected) {

        writeToLogFile("Server disconnected!");
        auto app = application();
        app->quit();
    }
}

DRunner::~DRunner()
{
    m_serverThread->quit();
    m_serverThread->wait();
    delete m_serverThread;
}
