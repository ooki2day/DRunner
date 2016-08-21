#ifndef DRUNNER_H
#define DRUNNER_H

#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QThread>

#include "server.h"
#include "processlauncher.h"

class DRunner : public QObject, public QtService<QCoreApplication>
{

    Q_OBJECT

public:
    DRunner(int argc, char **argv);
    ~DRunner();

public slots:
    void writeToLogFile(QString text);

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

private:
    Server *m_server;
    QThread *m_serverThread;
    ProcessLauncher *m_launcher;
    QThread *m_launcherThread;
    QFile *m_logFile;
};

#endif // MAINWINDOW_H
