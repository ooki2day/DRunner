#include "drunner.h"

DRunner::DRunner(int argc, char **argv) :
    QtService<QCoreApplication>(argc, argv, "DRunner")
{
    setServiceDescription("A dummy HTTP service implemented with Qt");
    setServiceFlags(QtServiceBase::CanBeSuspended);

    m_serverThread = new QThread();
    m_launcherThread = new QThread();
}

void DRunner::initLogFile() {

    m_logFile = new QFile(QDir::tempPath() + "/DRunner.log");
    if(m_logFile->open(QIODevice::WriteOnly)) {

        m_logFile->close();
        return;
    }

    auto app = application();
    logMessage(QObject::tr("Can't open the log file."));
    app->quit();
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

        QStringList list;
        while(!file.atEnd()) {

            list.append(file.readLine());
        }

        m_launcher->setAllowedList(list);

    } else {

        logMessage(QObject::tr("Can't open the file with allowed programs."));
    }
}

void DRunner::start() {

    serverInit();
    launcherInit();
    initLogFile();

    parseArgs(QCoreApplication::arguments());
}

void DRunner::serverInit() {

    m_server = new Server;
    m_server->moveToThread(m_serverThread);
    m_serverThread->start();

    connect(m_server, &Server::recvCommandFromSocket,
            this, &DRunner::writeToLogFile);
    connect(m_serverThread, &QThread::finished,
            m_server, &Server::deleteLater);
    connect(m_server, &Server::connectStateChanged,
            this, &DRunner::serverStateChanged);
}

void DRunner::launcherInit() {

    m_launcher = new ProcessLauncher;
    m_launcher->moveToThread(m_launcherThread);
    m_launcherThread->start();

    connect(m_serverThread, &QThread::finished,
            m_launcher, &ProcessLauncher::deleteLater);
    connect(m_server, &Server::parsedDataReady,
            m_launcher, &ProcessLauncher::startNewProcess);
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
    m_launcherThread->quit();
    m_serverThread->wait();
    m_launcherThread->wait();

    delete m_serverThread;
    delete m_launcherThread;
}
