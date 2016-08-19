#ifndef DRUNNER_H
#define DRUNNER_H

#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QThread>

#include "server.h"

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
    bool initLogFile();
    void parseArgs(const QStringList &list);
    void readAllowedPrograms(QFile &file);
    bool serverInitCurrectly();

private:
    Server *m_server;
    QFile *m_logFile;
    QStringList m_allowedProgramsList;
    QThread *m_serverThread;
};

#endif // MAINWINDOW_H
