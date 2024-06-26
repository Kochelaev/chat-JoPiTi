#include "XmlWriter.h"
#include <QtXml>
#include <app/user.h>
#include <QString>
#include "enum/MessageType.h"
#include <QTime>

using namespace Enum;

XmlWriter::XmlWriter()
{

}

QString XmlWriter::PrepareClientMessage(const QString &messageText, QString name, QString type)
{
    QDomDocument doc;
    QDomElement elem = doc.createElement(type);

    elem.appendChild(doc.createTextNode(messageText));

    elem.setAttribute("type", "message");

    if (name.isEmpty()) {
        name = User::instance().getName();
    }
    elem.setAttribute("name", name);

    elem.setAttribute("time", QTime::currentTime().toString());
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

QString XmlWriter::ImageMessage()
{
    QDomDocument doc;
    QDomElement elem = doc.createElement(MessageType::image);
    elem.setAttribute("name", User::instance().getName());

    doc.appendChild(elem);

    return doc.toString();
}

QString XmlWriter::OlegMessage()
{
    QDomDocument doc;
    QDomElement elem = doc.createElement(MessageType::oleg);
    doc.appendChild(elem);
    return doc.toString();
}


