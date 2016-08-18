#ifndef DRUNNER_H
#define DRUNNER_H

#include <QCoreApplication>
#include <QFile>
#include <QDir>

#include "server.h"

class DRunner : public QtService<QCoreApplication>
{
public:
    DRunner(int argc, char **argv);
    void writeToLogFile(QString text);
    ~DRunner();

protected:
    void start();

private:
    void initLogFile();

private:
    Server *m_server;
    QFile *m_logFile;
};

#endif // MAINWINDOW_H
