#include "server.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>
#include <QTcpServer>

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

    //layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(new QLabel("<H1>Сервер</H1>"));
    pvbxLayout->addWidget(m_ptxt);
    setLayout(pvbxLayout);
}

/*virtual*/ void Server::slotNewConnection()
{
    qDebug()<<"slot new connection";
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    activeConnections += pClientSocket;

    connect(pClientSocket, SIGNAL(disconnected()), pClientSocket, SLOT(deleteLater()));
    connect(pClientSocket, SIGNAL(disconnected()), this, SLOT(slotClientDisconnect()));
    connect(pClientSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));

    qDebug()<<"Соединие с сервером установлено!";
    sendTOClient(pClientSocket, "Соединие с сервером установлено!");
}

void Server::slotReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in (pClientSocket);

    in.setVersion(QDataStream::Qt_5_3);
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
        in >> time >> str;

        QString strMessage = time.toString() + ": " + str;
        m_ptxt->append(strMessage);

        m_nNExtBlockSize = 0;

        sendToAll(str);
    }
}

void Server::sendTOClient(QTcpSocket* pSocket, const QString& str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << quint16(0) << QTime::currentTime() << str;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket->write(arrBlock);
}

void Server::sendToAll(const QString &str)
{
    foreach (auto client, activeConnections) {
        qDebug() << "send to all";
        sendTOClient(client, str);
    }
}


void Server::slotClientDisconnect()
{
    qDebug() << "client disc";
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    auto iterator = qFind(activeConnections.begin(), activeConnections.end(), pClientSocket);
    if (iterator != activeConnections.end()) {
        activeConnections.erase(iterator);
        qDebug() << "client Disconnetcet";
    }
}
