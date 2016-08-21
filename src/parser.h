#ifndef PARSER_H
#define PARSER_H

#include <QRunnable>
#include <QList>

#include "utils.h"

class Parser : public QObject, public QRunnable
{
    Q_OBJECT
public:
    Parser(qintptr descriptor, const QByteArray &data);

signals:
    void socketDataReady(const Utils::SocketData &data);

private:
    void run();

private:
    Utils::ParsedData parseData(const QByteArray &data);

private:
    qintptr m_desciptor;
    QByteArray m_data;
};

#endif // PARSER_H
