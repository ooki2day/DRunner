#ifndef UTILS_H
#define UTILS_H

#include <QByteArray>
#include <QStringList>
#include <QThread>

namespace Utils {

struct ParsedData {

    QString program;
    QStringList args;
    qint32 timeout = 0;
};

struct SocketData {

    qintptr descriptor = 0;
    ParsedData data;
};
}


#endif // UTILS_H
