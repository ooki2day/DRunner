#include "drunner.h"

#include <QDebug>

DRunner::DRunner(int argc, char **argv) :
    QtService<QCoreApplication>(argc, argv, "DRunner")
{
    setServiceDescription("A dummy HTTP service implemented with Qt");
    setServiceFlags(QtServiceBase::CanBeSuspended);

    initLogFile();
}

void DRunner::initLogFile() {

    m_logFile = new QFile(QDir::tempPath() + "/log");
    m_logFile->open(QIODevice::WriteOnly | QIODevice::Text);
}

void DRunner::writeToLogFile(QString text) {

    m_logFile->write(text.toLocal8Bit());
}

void DRunner::start() {

    QCoreApplication *app = application();

    const QStringList args = QCoreApplication::arguments();

    m_server = new Server(app);

    if(!m_server->isListening()) {

        logMessage(QObject::tr("Failed to start the server"), QtServiceBase::Error);
        app->quit();
    }

    if(!m_logFile->isOpen()) {

        logMessage(QObject::tr("Can't open the log file. Close Program"));
        app->quit();
    }

    writeToLogFile("started");
}

DRunner::~DRunner()
{
    writeToLogFile("stopped");
    m_logFile->close();
}
