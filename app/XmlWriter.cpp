#include "XmlWriter.h"
#include <QDebug>
#include <QtXml>
#include <app/user.h>
#include <QString>
#include "enum/MessageType.h"

using namespace Enum;

XmlWriter::XmlWriter()
{
    qDebug() << "Xml";
}

QString XmlWriter::PrepareClientMessage(const QString &messageText)
{
    QDomDocument doc;
    QDomElement elem = doc.createElement("message");

    elem.appendChild(doc.createTextNode(messageText));

    elem.setAttribute("type", "message");
    elem.setAttribute("name", User::instance().getName());
    doc.appendChild(elem);

    return doc.toString();
}

QString XmlWriter::ClientName()
{
    QDomDocument doc;
    QDomElement elem = doc.createElement(MessageType::sendName);
    elem.setAttribute("name", User::instance().getName());

    doc.appendChild(elem);

    return doc.toString();
}
