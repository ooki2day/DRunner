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
    bool initLogFile();
    void parseArgs(const QStringList &list);
    void readAllowedPrograms(QFile &file);

private:
    Server *m_server;
    QFile *m_logFile;
    QStringList m_allowedProgramsList;
};

#endif // MAINWINDOW_H
