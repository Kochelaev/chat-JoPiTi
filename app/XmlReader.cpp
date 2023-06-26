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
    qDebug() << message;
}


