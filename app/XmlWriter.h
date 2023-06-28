#ifndef XMLWRITER_H
#define XMLWRITER_H

#include <QString>
#include <QtXml>
#include "enum/MessageType.h"

namespace app {

    class XmlWriter
    {
    public:
        XmlWriter();

        static QString PrepareClientMessage(const QString &messageText, QString name = "", QString type = "message");
        static QString ClientName();
        static QString ImageMessage();

        template <typename container>
        static QString createNameList(const container &names) //const &
        {
            QDomDocument doc;
            QDomElement parrentElem = doc.createElement(Enum::MessageType::namesList);
            foreach (QString name, names) {
                QDomText domText= doc.createTextNode(name);
                QDomElement childElem = doc.createElement("name");
                childElem.appendChild(domText);

                parrentElem.appendChild(childElem);
            }
            doc.appendChild(parrentElem);

            return doc.toString();
        }

    };
}

using namespace app;

#endif // XMLWRITER_H
