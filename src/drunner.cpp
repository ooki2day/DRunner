#include "drunner.h"

#include <QDebug>

DRunner::DRunner(int argc, char **argv) :
    QtService<QCoreApplication>(argc, argv, "DRunner")
{
    setServiceDescription("A dummy HTTP service implemented with Qt");
    setServiceFlags(QtServiceBase::CanBeSuspended);
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

        qDebug() << m_logFile->write(text.toLocal8Bit());
        m_logFile->close();
    }

    logMessage(QObject::tr("Can't write to log file!"));
}

void DRunner::parseArgs(const QStringList &list) {

    QFile file;
    for(int i = 1; i < list.size(); ++i) {

        file.setFileName(list.at(i));
        if(file.exists()) {

            readAllowedPrograms(file);
            file.close();
            return;
        }
    }

    logMessage(QObject::tr("The file with allowed programs not exist."));
}

void DRunner::readAllowedPrograms(QFile &file) {


    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {

        while(!file.atEnd()) {

            m_allowedProgramsList.append(file.readLine());
        }
    } else {

        logMessage(QObject::tr("Can't open the file with allowed programs."));
    }
}

void DRunner::start() {

    QCoreApplication *app = application();
    if(!initLogFile()) {

        logMessage(QObject::tr("Can't open the log file."));
        app->quit();
        return;
    }

    m_server = new Server(app);

    if(!m_server->isListening()) {

        logMessage(QObject::tr("Failed to start the server"), QtServiceBase::Error);
        app->quit();
        return;
    }

    parseArgs(QCoreApplication::arguments());
}

DRunner::~DRunner()
{
    writeToLogFile("stopped \n");
}
