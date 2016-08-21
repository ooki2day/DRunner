#include "parser.h"

#include <QDebug>

Parser::Parser(qintptr descriptor, const QByteArray &data):
    m_desciptor(descriptor),
    m_data(data)
{

}

void Parser::run() {

    Utils::SocketData sockData;
    sockData.data = parseData(m_data);
    sockData.descriptor = m_desciptor;

    emit socketDataReady(sockData);
}

Utils::ParsedData Parser::parseData(const QByteArray &data) {

    auto list = data.split(' ');
    bool ok = false;
    int timeout = 0;

    Utils::ParsedData parsed;
    parsed.program = list.at(0).trimmed().simplified();

    for(int i = 1; i < list.size(); ++i) {

        timeout = list.at(i).toInt(&ok);
        if(ok && timeout > 0) {

            parsed.timeout = timeout;
        }
        parsed.args << list.at(i).simplified();
    }

    return parsed;
}
