#include "processlauncher.h"

#include <QDebug>

ProcessLauncher::ProcessLauncher(QObject *parent) :
    QObject(parent)
{

}

void ProcessLauncher::startNewProcess(const Utils::SocketData &data) {

    QProcess *proc = new QProcess(this);
    proc->setProgram(data.data.program);
    proc->setArguments(data.data.args);
    proc->start();
}
