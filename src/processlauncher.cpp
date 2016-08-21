#include "processlauncher.h"

#include <QDebug>

ProcessLauncher::ProcessLauncher()
{
}

void ProcessLauncher::startNewProcess(const Utils::SocketData &data) {

    QProcess *proc = new QProcess(this);
    proc->setProgram(data.data.program);
    proc->setArguments(data.data.args);
    proc->start();
}

bool ProcessLauncher::isProgramAllowed(const QString &program) {

    return m_allowedProgrammsList.contains(program);
}

ProcessLauncher::~ProcessLauncher() {

}
