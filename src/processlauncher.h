#ifndef PROCESSLAUNCHER_H
#define PROCESSLAUNCHER_H

#include <QObject>
#include <QProcess>

#include "utils.h"

class ProcessLauncher : public QObject
{
    Q_OBJECT

public:
    ProcessLauncher(QObject *parent = 0);

public slots:
    void startNewProcess(const Utils::SocketData &data);
};

#endif // PROCESSLAUNCHER_H
