#ifndef PROCESSLAUNCHER_H
#define PROCESSLAUNCHER_H

#include <QProcess>
#include <QObject>
#include <QTimer>
#include <QObjectUserData>
#include <QMap>

#include "utils.h"

class ProcessLauncher : public QObject
{
    Q_OBJECT
public:
    ProcessLauncher();
    void setAllowedList(const QStringList &list) { m_allowedProgrammsList = list;
                                                   qSort(m_allowedProgrammsList); }

    ~ProcessLauncher();

signals:
    void programStarted(qintptr descriptor, const QString &program);
    void programStartFailed(qintptr descriptor, const QString &fail, QProcess::ProcessError error);
    void programSelfTerminated(qintptr descriptor, const QString &program);
    void programTimeout(qintptr descriptor, const QString &program);
    void recvDataFromProcess(qintptr descriptor, const QByteArray &data);

public slots:
    void startNewProcess(const Utils::SocketData &data);

private slots:
    void processFinished(int exitCode, QProcess::ExitStatus status);
    void processError(QProcess::ProcessError error);
    void processStarted();
    void timeout();
    void readFromProcess();

private:
    bool isProgramAllowed(const QString &program);
    void stopAllProcesses();

private:
    QStringList m_allowedProgrammsList;
    QMap<int, QProcess*> m_timeredProcessMap;
    QList<QProcess*> m_activeProcessesList;
};

#endif // PROCESSLAUNCHER_H
