#include "server.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>
#include <QTcpServer>
#include <app/XmlWriter.h>
#include "app/XmlReader.h"
#include "enum/MessageType.h"

using namespace Enum;
using namespace app;

Server::Server(int nPort, QWidget* pwgt /*= 0*/) : QWidget(pwgt)
                                             , m_nNExtBlockSize(0)
{
    m_ptcpServer = new QTcpServer(this);
    if (!m_ptcpServer->listen(QHostAddress::Any, nPort)) {
        QMessageBox::critical(0,
                              "Server Error",
                              "Невозможно запуститить сервер:"
                              );

        m_ptcpServer->close();
        return;
    }

    connect(m_ptcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));

    m_ptxt = new QTextEdit;
    m_ptxt->setReadOnly(true);

    m_nameList = new QTextEdit("<H4>Пользователи в сети: <br></H4>");
    m_nameList->setReadOnly(true);

    //layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(new QLabel("<H1>Сервер</H1>"));
    pvbxLayout->addWidget(m_nameList);
    pvbxLayout->addWidget(m_ptxt);
    setLayout(pvbxLayout);
}

/*virtual*/ void Server::slotNewConnection()
{
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    activeConnections += pClientSocket;

    connect(pClientSocket, SIGNAL(disconnected()), this, SLOT(slotClientDisconnect()));
    connect(pClientSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
}

void Server::slotReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in (pClientSocket);

    in.setVersion(QDataStream::Qt_5_11);
    for (;;) {
        if (!m_nNExtBlockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNExtBlockSize;
        }

        if (pClientSocket->bytesAvailable() < m_nNExtBlockSize) {
            break;
        }

        QTime time;
        QString str;
        QImage image;
        in >> time >> str >> image;

//        if (!image.isNull()) {  //!!!!!!
//            QWidget* w = new QWidget();
//            QPalette pal;
//            pal.setBrush(w->backgroundRole(), QBrush(image));
//            w->setPalette(pal);
//            w->setAutoFillBackground(true);
//            w->show();
//        }


        QString strMessage = time.toString() + ": " + str;
        m_ptxt->append(str);

        m_nNExtBlockSize = 0;

        requestProcessing(str, pClientSocket, image);
    }
}

void Server::sendTOClient(QTcpSocket* pSocket, const QString& str, QImage image)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_5_11);
    out << quint16(0) << QTime::currentTime() << str << image;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket->write(arrBlock);
}

void Server::sendToAll(const QString &str, QImage image)
{
    foreach (auto client, activeConnections) {
        sendTOClient(client, str, image);
    }
}

void Server::requestProcessing(const QString &in, QTcpSocket* sender, QImage image)
{
    QString messageType = app::XmlReader::getMessageType(in);

    if (messageType == Enum::MessageType::sendName) {
        this->processSendClientName(in, sender);
    }else if (messageType == Enum::MessageType::message) {
        this->processMessage(in, sender);
    } else if (messageType == Enum::MessageType::image) {
        this->processImage(in, sender, image);
    }
}

void Server::processMessage(const QString &in, QTcpSocket *sender)
{
    QString name = clientNames[sender];
    QString text = app::XmlReader::getMessageText(in);
    QString xmlMessage = app::XmlWriter::PrepareClientMessage(text, name);

    sendToAll(xmlMessage);
}

void Server::processSendClientName(const QString &in, QTcpSocket *sender)
{
    QString name = app::XmlReader::getClientName(in);

    clientNames[sender] = name;

    refreshNameList();
    sendNameList();
}

void Server::processImage(const QString &message, QTcpSocket *sender, QImage &image)
{
    QString name = clientNames[sender];
    QString HtmlMessage = app::XmlWriter::PrepareClientMessage("", name, Enum::MessageType::image);

    sendToAll(HtmlMessage, image);
}

void Server::refreshNameList()
{
    QString names = "";
    foreach (QString name, clientNames) {
        names += name + "\n";
    }
    m_nameList->setText(names);
}

void Server::sendNameList()
{
    QString nameList = XmlWriter::createNameList(clientNames);
    sendToAll(nameList);
}

void Server::slotClientDisconnect()
{
    qDebug() << "client disc";
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();

    auto iterator = std::find(activeConnections.begin(), activeConnections.end(), pClientSocket);
    if (iterator != activeConnections.end()) {
        activeConnections.erase(iterator);
    }

    auto mapIterator = clientNames.find(pClientSocket);
    if (mapIterator != clientNames.end()) {
        clientNames.erase(mapIterator);
    }

    refreshNameList();
    sendNameList();
}

