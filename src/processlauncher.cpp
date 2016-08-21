#include "processlauncher.h"

#include <QDebug>

ProcessLauncher::ProcessLauncher()
{
}

void ProcessLauncher::startNewProcess(const Utils::SocketData &data) {

    if(!isProgramAllowed(data.data.program))
        return;

    QProcess *proc = new QProcess(this);
    proc->setProgram(data.data.program);
    proc->setArguments(data.data.args);

    connect(proc, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(processFinished(int,QProcess::ExitStatus)));

    proc->start();

    if(data.data.timeout > 0) {

        QTimer *timer = new QTimer(this);
        timer->setInterval(data.data.timeout);

        connect(timer, &QTimer::timeout,
                proc, &QProcess::close);
        connect(timer, &QTimer::timeout,
                timer, &QTimer::deleteLater);
        connect(timer, &QTimer::timeout,
                this, &ProcessLauncher::timeout);

        timer->start();
    }
}

void ProcessLauncher::timeout() {

    qDebug() << "timeout";
}

void ProcessLauncher::processFinished(int exitCode, QProcess::ExitStatus status) {

    Q_UNUSED(exitCode)
    Q_UNUSED(status)

    qDebug() << exitCode << "   " << status;

    auto proc = (QProcess*)sender();

    if(m_activeProcessesList.contains(proc))
        m_activeProcessesList.removeOne(proc);

    proc->deleteLater();
}

bool ProcessLauncher::isProgramAllowed(const QString &program) {

    return m_allowedProgrammsList.contains(program);
}

void ProcessLauncher::stopAllProcesses() {

    for(auto i : m_activeProcessesList) {

        i->close();
    }
}

ProcessLauncher::~ProcessLauncher() {

    stopAllProcesses();
}
