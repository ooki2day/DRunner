#include "parser.h"

Parser::Parser(qintptr descriptor, const QByteArray &data):
    m_desciptor(descriptor),
    m_data(data)
{

}

void Parser::run() {
    Utils::SocketData sockData;
    sockData.m_data = parseData(m_data);
    sockData.m_descriptor = m_desciptor;

    emit socketDataReady(sockData);
}

Utils::ParsedData Parser::parseData(const QByteArray &data) {
    auto list = data.split(' ');
    bool ok = false;
    int timeout = 0;

    Utils::ParsedData parsed;
    parsed.m_program = list.at(0).trimmed().simplified();

    for(int i = 1; i < list.size(); ++i) {
        timeout = list.at(i).simplified().toInt(&ok);
        if(ok && timeout > 0) {
            parsed.m_timeout = timeout;
        } else {
            parsed.m_args << list.at(i).simplified();
        }
    }

    return parsed;
}
