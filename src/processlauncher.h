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
    void setAllowedList(const QStringList &list) { m_allowedProgrammsList = list; qSort(m_allowedProgrammsList); }

    ~ProcessLauncher();

public slots:
    void startNewProcess(const Utils::SocketData &data);

private slots:
    void processFinished(int exitCode, QProcess::ExitStatus status);
    void timeout();

private:
    bool isProgramAllowed(const QString &program);
    void stopAllProcesses();

private:
    QStringList m_allowedProgrammsList;
    QList<QProcess*> m_activeProcessesList;
};

#endif // PROCESSLAUNCHER_H
