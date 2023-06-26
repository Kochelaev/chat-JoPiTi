#ifndef XMLREADER_H
#define XMLREADER_H

#include<QString>

namespace app {

    class XmlReader
    {
    public:
        XmlReader();

        static QString getMessageType(const QString& message);
        static QString getClientName(const QString& message);
        static QString getMessageText(const QString& in);
        static QVector<QString> getNamesList(const QString& message);
        static QString getHtmlMessage(const QString& message);
    };
}

#endif // XMLREADER_H
