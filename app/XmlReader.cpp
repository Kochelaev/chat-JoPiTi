#include "XmlReader.h"

#include <QXmlStreamReader>
#include <QDebug>
#include <QtXml>
#include <QDataStream>
#include "enum/MessageType.h"

using namespace app;

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

QString XmlReader::getClientName(const QString &message)
{
    QDomDocument doc;
    doc.setContent(message);

    QDomElement nameElement = doc.firstChildElement(Enum::MessageType::sendName);

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

QString XmlReader::getHtmlMessage(const QString &message)
{
    QDomDocument doc;
    doc.setContent(message);

    QDomElement messageElem = doc.firstChildElement(Enum::MessageType::message);
    QString time = messageElem.attribute("time");
    QString name = messageElem.attribute("name");
    QString text = messageElem.text();

    QString htmlMessage =
            "<b class=\"time\">" + time +
            "</b> <b class=\"name\">" + name +
            ":</b> <b class=\"messsage\">" + text + "</b>";

    return htmlMessage;
}


