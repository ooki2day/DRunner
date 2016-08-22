#ifndef UTILS_H
#define UTILS_H

#include <QByteArray>
#include <QStringList>
#include <QThread>
#include <QHostAddress>

namespace Utils {

struct ParsedData {

    QString m_program;
    QStringList m_args;
    qint32 m_timeout = 0;
};

struct SocketData {

    qintptr m_descriptor = 0;
    QHostAddress m_address;
    quint16 m_port = 0;
    ParsedData m_data;
};

struct LogData {

    qintptr m_desctiptor = 0;
    QHostAddress m_address;
    quint16 m_port = 0;
    quint32 m_bytesWritten = 0;
    quint32 m_bytesRead = 0;
    QStringList m_startedPrograms;
    QStringList m_StartProgramErrors;
    QStringList m_selfTerminatedPrograms;
    QStringList m_timeoutTerminatedPrograms;

    LogData() {}
    LogData(qintptr descr, QHostAddress addr, quint16 port, int dataSize) :
        m_desctiptor(descr), m_address(addr), m_port(port), m_bytesRead(dataSize) {}
};
}


#endif // UTILS_H
