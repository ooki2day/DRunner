#ifndef LOGCOLLECTOR_H
#define LOGCOLLECTOR_H

#define LOGFILENAME "DRunner.log"

#include <QObject>
#include <QHash>
#include <QMutex>
#include <QProcess>
#include <QFile>
#include <QDir>

#include "utils.h"

class LogCollector : public QObject
{
public:
    explicit LogCollector(QObject *parent = 0);
    ~LogCollector();

public slots:
    void recvDataFromClient(qintptr descriptor, QHostAddress address, quint16 port, int dataSize);
    void sendDataToClient(qintptr descriptor, const QByteArray &data);
    void programStarted(qintptr descriptor, const QString &program);
    void programStartFailed(qintptr descriptor, const QString &program, QProcess::ProcessError error);
    void programSelfTermitated(qintptr descriptor, const QString &program);
    void programTimeout(qintptr descriptor, const QString &program);
    void writeLog(qintptr descriptor);

private:
    QHash<qintptr, Utils::LogData> m_logDataHash;
};

#endif // LOGCOLLECTOR_H
