#include "XmlReader.h"

#include <QXmlStreamReader>
#include <QtXml>
#include <QDataStream>
#include "enum/MessageType.h"
#include "app/user.h"

using namespace app;
using namespace Enum;

XmlReader::XmlReader()
{

}

QString XmlReader::getMessageType(const QString &message)
{
    QString result = "";
    QXmlStreamReader reader(message.toUtf8());

    do {
        reader.readNext();
        if (!reader.name().isEmpty()) {
            result = reader.name().toString();
            break;
        }
    } while (!reader.atEnd());

    return result;
}

QString XmlReader::getClientName(const QString &message, QString messageType)
{
    QDomDocument doc;
    doc.setContent(message);

    messageType = messageType.isEmpty()? MessageType::sendName : messageType;
    QDomElement nameElement = doc.firstChildElement(messageType);

    return nameElement.attribute("name");
}

QString XmlReader::getMessageText(const QString &message)
{
    QDomDocument doc;
    doc.setContent(message);

    QDomElement messageElem = doc.firstChildElement(Enum::MessageType::message);

    return messageElem.text();
}

QVector<QString> XmlReader::getNamesList(const QString &message)
{
    QVector<QString> result;

    QDomDocument doc;
    doc.setContent(message);

    QDomElement namesListElem = doc.firstChildElement(Enum::MessageType::namesList);
    QDomNodeList namesNodeList = namesListElem.childNodes();
    for(int i=0; i <= namesNodeList.size(); i++) {
        QDomNode item = namesNodeList.item(i);
        if (item.isElement()) {
            result += item.toElement().text();
        }
    }

    return result;
}

QString XmlReader::getHtmlMessage(const QString &message, QString messageType)
{
    QDomDocument doc;
    doc.setContent(message);

    messageType = messageType.isEmpty()? MessageType::message : messageType;

    QDomElement messageElem = doc.firstChildElement(messageType);
    QString time = messageElem.attribute("time");
    QString name = messageElem.attribute("name");
    QString text = messageElem.text().replace("\n", "<br>");

    QString nameColor;
    if (name == app::User::instance().getName()) {
        nameColor = "green";
    } else {
        nameColor = "blue";
    }

    QString htmlMessage =
            "<p style=\"margin-left: 0px;\"> <small class=\"time\" style=\"color: #B1B1B1\">" + time +
            "</small> <b class=\"name\" style=\"color:" + nameColor + "\">" + name +
            ":</b> <o class=\"messsage\">" + text + "</o> </p>";

    return htmlMessage;
}

QString XmlReader::getMessageTime(const QString & message, QString messageType)
{
    QDomDocument doc;
    doc.setContent(message);

    messageType = messageType.isEmpty()? MessageType::message : messageType;
    QDomElement messageElem = doc.firstChildElement(messageType);
    QString time = messageElem.attribute("time");
    return time;
}


