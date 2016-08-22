#include "drunner.h"

DRunner::DRunner(int argc, char **argv) :
    QtService<QCoreApplication>(argc, argv, "DRunner")
{
    setServiceDescription("A dummy HTTP service implemented with Qt");
    setServiceFlags(QtServiceBase::CanBeSuspended);

    m_serverThread = new QThread();
    m_launcherThread = new QThread();
    m_logCollectorThread = new QThread();

    qRegisterMetaType<qintptr>("qintptr");
}

void DRunner::initLogFile() {

    QFile file(QDir::tempPath() + "/" + QString(LOGFILENAME));
    if(file.open(QIODevice::WriteOnly)) {

        file.close();
        return;
    }

    auto app = application();
    logMessage(QObject::tr("Can't open the log file."));
    app->quit();
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

            list.append(file.readLine().simplified());
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
    logCollectorInit();

    parseArgs(QCoreApplication::arguments());
}

void DRunner::serverInit() {

    m_server = new Server;
    m_server->moveToThread(m_serverThread);
    m_serverThread->start();

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
    connect(m_launcher, &ProcessLauncher::recvDataFromProcess,
            m_server, &Server::sendToSocket);
}

void DRunner::logCollectorInit() {

    m_logCollector = new LogCollector;
    m_logCollector->moveToThread(m_logCollectorThread);
    m_logCollectorThread->start();

    connect(m_logCollectorThread, &QThread::finished,
            m_logCollector, &LogCollector::deleteLater);

    connect(m_server, &Server::socketDisconnected,
            m_logCollector, &LogCollector::writeLog);
    connect(m_server, &Server::recvDataFromSocket,
            m_logCollector, &LogCollector::recvDataFromClient);

    connect(m_launcher, &ProcessLauncher::recvDataFromProcess,
            m_logCollector, &LogCollector::sendDataToClient);
    connect(m_launcher, &ProcessLauncher::programSelfTerminated,
            m_logCollector, &LogCollector::programSelfTermitated);
    connect(m_launcher, &ProcessLauncher::programStarted,
            m_logCollector, &LogCollector::programStarted);
    connect(m_launcher, &ProcessLauncher::programStartFailed,
            m_logCollector, &LogCollector::programStartFailed);
    connect(m_launcher, &ProcessLauncher::programTimeout,
            m_logCollector, &LogCollector::programTimeout);
}

void DRunner::serverStateChanged(bool connected) {

    if(!connected) {

        logMessage("Server disconnected!");
        auto app = application();
        app->quit();
    }
}

DRunner::~DRunner()
{
    m_serverThread->quit();
    m_launcherThread->quit();
    m_logCollectorThread->quit();
    m_serverThread->wait();
    m_launcherThread->wait();
    m_logCollectorThread->wait();

    delete m_serverThread;
    delete m_launcherThread;
    delete m_logCollectorThread;
}
