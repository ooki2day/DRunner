#ifndef PROCESSLAUNCHER_H
#define PROCESSLAUNCHER_H

#include <QProcess>
#include <QObject>

#include "utils.h"

class ProcessLauncher : public QObject
{
public:
    ProcessLauncher();
    void setAllowedList(const QStringList &list) { m_allowedProgrammsList = list; }

    ~ProcessLauncher();

public slots:
    void startNewProcess(const Utils::SocketData &data);

private:
    bool isProgramAllowed(const QString &program);

private:
    QStringList m_allowedProgrammsList;
};

#endif // PROCESSLAUNCHER_H
