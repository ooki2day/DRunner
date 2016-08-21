#include "drunner.h"

int main(int argc, char **argv)
{
    DRunner daemon(argc, argv);

    auto conn = QObject::connect(&daemon, &DRunner::destroyed,
                                 &daemon, &DRunner::deleteLater);

    return daemon.exec();
}
