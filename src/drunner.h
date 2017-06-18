#ifndef DRUNNER_H
#define DRUNNER_H

#include <QCoreApplication>
#include <QThread>

#include "server.h"
#include "processlauncher.h"
#include "logcollector.h"

class DRunner : public QObject, public QtService<QCoreApplication>
{
    Q_OBJECT

public:
    DRunner(int argc, char **argv);
    ~DRunner();

protected:
    void start();

private slots:
    void serverStateChanged(bool connected);

private:
    void initLogFile();
    void parseArgs(const QStringList &list);
    void readAllowedProgramsFromFile(QFile &file);
    void serverInit();
    void launcherInit();
    void logCollectorInit();

private:
    Server *m_server;
    QThread *m_serverThread;

    ProcessLauncher *m_launcher;
    QThread *m_launcherThread;

    LogCollector *m_logCollector;
    QThread *m_logCollectorThread;
};

#endif // MAINWINDOW_H
