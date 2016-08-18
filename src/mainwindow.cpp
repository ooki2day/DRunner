#include "mainwindow.h"
#include "ui_mainwindow.h"

DRunner::DRunner(int argc, char **argv) :
    QtService<QCoreApplication>(argc, argv, "DRunner")
{
    setServiceDescription("A dummy HTTP service implemented with Qt");
    setServiceFlags(QtServiceBase::CanBeSuspended);
}

void DRunner::start() {

    QCoreApplication *app = application();

    const QStringList args = QCoreApplication::arguments();
}

DRunner::~DRunner()
{
}
