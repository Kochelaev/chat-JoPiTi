#ifndef XMLREADER_H
#define XMLREADER_H

#include <QString>
#include "enum/MessageType.h"

using namespace Enum;

namespace app {

    class XmlReader
    {
    public:
        XmlReader();

        static QString getMessageType(const QString& message);
        static QString getClientName(const QString& message, QString messageType = "");
        static QString getMessageText(const QString& in);
        static QVector<QString> getNamesList(const QString& message);
        static QString getHtmlMessage(const QString& message, QString messageType = "");
        static QString getMessageTime(const QString& message, QString messageType = "");
    };
}

#endif // XMLREADER_H
