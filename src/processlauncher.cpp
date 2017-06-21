#include "processlauncher.h"

ProcessLauncher::ProcessLauncher() {
    qRegisterMetaType<QProcess::ProcessError>("QProcess::ProcessError");
}

void ProcessLauncher::startNewProcess(const Utils::SocketData &data) {
    if(!isProgramAllowed(data.m_data.m_program))
        return;

    QProcess *proc = new QProcess(this);
    proc->setProgram(data.m_data.m_program);
    proc->setArguments(data.m_data.m_args);

    connect(proc, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(processFinished(int,QProcess::ExitStatus)));
    connect(proc, SIGNAL(errorOccurred(QProcess::ProcessError)),
            this, SLOT(processError(QProcess::ProcessError)));
    connect(proc, SIGNAL(started()),
            this, SLOT(processStarted()));
    connect(proc, SIGNAL(readyRead()),
            this, SLOT(readFromProcess()));

    proc->setProperty("descriptor", QVariant(data.m_descriptor));
    proc->start();

    if(data.m_data.m_timeout > 0) {
        QTimer *timer = new QTimer(this);
        timer->setInterval(data.m_data.m_timeout);

        connect(timer, &QTimer::timeout,
                timer, &QTimer::deleteLater);
        connect(timer, &QTimer::timeout,
                this, &ProcessLauncher::timeout);

        timer->start();
        m_timeredProcessMap[timer->timerId()] = proc;
    }
}

void ProcessLauncher::readFromProcess() {
    auto proc = (QProcess*)sender();
    auto data = proc->readAll();
    emit recvDataFromProcess(proc->property("descriptor").toInt(), data);
}

void ProcessLauncher::processStarted() {
    auto proc = (QProcess*)sender();
    emit programStarted(proc->property("descriptor").toInt(), proc->program());
}

void ProcessLauncher::processError(QProcess::ProcessError error) {
    if(error == QProcess::ProcessError::FailedToStart) {
        auto proc = (QProcess*)sender();
        emit programStartFailed(proc->property("descriptor").toInt(),
                                proc->program() + proc->errorString(), error);
    }
}

void ProcessLauncher::timeout() {
    auto timer = (QTimer*)sender();

    if(m_timeredProcessMap.contains(timer->timerId())) {
        auto proc = m_timeredProcessMap[timer->timerId()];
        m_activeProcessesList.removeOne(proc);
        proc->close();
        emit programTimeout(proc->property("descriptor").toInt(), proc->program());
    }
}

void ProcessLauncher::processFinished(int exitCode, QProcess::ExitStatus status) {
    Q_UNUSED(exitCode)
    Q_UNUSED(status)

    auto proc = (QProcess*)sender();
    if(!proc)
        return;

    if(m_activeProcessesList.contains(proc)) {
        m_activeProcessesList.removeOne(proc);
        emit programSelfTerminated(proc->property("descriptor").toInt(), proc->program());
    }

    proc->deleteLater();
}

bool ProcessLauncher::isProgramAllowed(const QString &program) {
    return m_allowedProgrammsList.contains(program);
}

void ProcessLauncher::stopAllProcesses() {
    for(auto i : m_activeProcessesList) {
        i->close();
    }
    qDeleteAll(m_activeProcessesList);
}

ProcessLauncher::~ProcessLauncher() {
    qDeleteAll(m_timeredProcessMap);
    stopAllProcesses();
}
