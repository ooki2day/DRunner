#include "logcollector.h"

LogCollector::LogCollector(QObject *parent) :
    QObject(parent)
{

}

void LogCollector::recvDataFromClient(qintptr descriptor, QHostAddress address, quint16 port, int dataSize) {

    if(m_logDataHash.contains(descriptor)) {

        m_logDataHash[descriptor].m_bytesRead += dataSize;
        return;
    }

    Utils::LogData data { descriptor, address, port, dataSize };
    m_logDataHash[descriptor] = data;
}

void LogCollector::sendDataToClient(qintptr descriptor, const QByteArray &data) {

    if(m_logDataHash.contains(descriptor)) {

        m_logDataHash[descriptor].m_bytesWritten += data.size();
    }
}

void LogCollector::programStarted(qintptr descriptor, const QString &program) {

    if(m_logDataHash.contains(descriptor)) {

        m_logDataHash[descriptor].m_startedPrograms << program;
    }
}

void LogCollector::programStartFailed(qintptr descriptor, const QString &program, QProcess::ProcessError error) {

    if(m_logDataHash.contains(descriptor)) {

        QString str(program);
        str.append(": " + error);
        m_logDataHash[descriptor].m_StartProgramErrors << program;
    }
}

void LogCollector::programSelfTermitated(qintptr descriptor, const QString &program) {

    if(m_logDataHash.contains(descriptor)) {

        m_logDataHash[descriptor].m_selfTerminatedPrograms << program;
    }
}

void LogCollector::programTimeout(qintptr descriptor, const QString &program) {

    if(m_logDataHash.contains(descriptor)) {

        m_logDataHash[descriptor].m_timeoutTerminatedPrograms << program;
    }
}

void LogCollector::writeLog(qintptr descriptor) {

    if(m_logDataHash.contains(descriptor)) {

        auto logData = m_logDataHash.take(descriptor);
        QString text(QString(logData.m_address.toString() + " "
                             + QString::number(logData.m_port) + ":\n"));

        text.append(QObject::tr("\nStarted programms:\n"));
        for(auto i : logData.m_startedPrograms) {

            text.append("    " + i + "\n");
        }

        text.append(QObject::tr("\nFailed to start programms:\n"));
        for(auto i : logData.m_StartProgramErrors) {

            text.append("    " + i + "\n");
        }

        text.append(QObject::tr("\nSelf-terminated programms:\n"));
        for(auto i : logData.m_selfTerminatedPrograms) {

            text.append("    " + i + "\n");
        }

        text.append(QObject::tr("\nTimeouted programms:\n"));
        for(auto i : logData.m_timeoutTerminatedPrograms) {

            text.append("    " + i + "\n");
        }

        text.append("\nStats:\n");
        text.append("    " + QObject::tr("Started: ") + QString::number(logData.m_startedPrograms.size()) + "\n");
        text.append("    " + QObject::tr("Failed to start: ") + QString::number(logData.m_StartProgramErrors.size()) + "\n");
        text.append("    " + QObject::tr("Terminated: ") + QString::number(logData.m_selfTerminatedPrograms.size()) + "\n");
        text.append("    " + QObject::tr("Timeouted: ") + QString::number(logData.m_timeoutTerminatedPrograms.size()) + "\n");

        text.append(QObject::tr("\nTraffic stats:\n"));
        text.append("    " + QObject::tr("Recieved bytes: ") + QString::number(logData.m_bytesRead) + "\n");
        text.append("    " + QObject::tr("Sended bytes: ") + QString::number(logData.m_bytesWritten) + "\n");

        text.append("==========================================================================");

        QFile file(QDir::tempPath() + "/" + LOGFILENAME);
        if(file.open(QIODevice::Append | QIODevice::Text)) {

            file.write(text.toLocal8Bit());
            file.close();
        }
    }
}

LogCollector::~LogCollector() {

    for(auto i : m_logDataHash) {

        writeLog(i.m_desctiptor);
    }
}
