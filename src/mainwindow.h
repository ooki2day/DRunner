#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define PORT 12345

#include "qt-solutions/qtservice/src/qtservice.h"

class DRunner : public QtService<QCoreApplication>
{
    Q_OBJECT

protected:
    void start();

private:


public:
    DRunner(int argc, char **argv);
    ~DRunner();
};

#endif // MAINWINDOW_H
