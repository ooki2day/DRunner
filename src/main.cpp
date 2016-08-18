#include "mainwindow.h"

int main(int argc, char **argv)
{
    DRunner daemon(argc, argv);

    return daemon.exec();
}
