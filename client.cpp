#include "client.h"
#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>
#include <QPushButton>


Client::Client(const QString& strHost, int nPort, QWidget* pwgt/*=0*/)
{
    m_pTcpSocket = new QTcpSocket(this);

    m_pTcpSocket->connectToHost(strHost, nPort);
    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));

    m_ptxtInfo = new QTextEdit;
    m_ptxtInput = new QLineEdit;

    m_ptxtInfo->setReadOnly(true);

    QPushButton* pcmd = new QPushButton("&Отправить");
    connect(pcmd, SIGNAL(clicked()), SLOT(slotSentToServer()));
    connect(m_ptxtInput, SIGNAL(returnPressed()), this, SLOT(slotSentToServer()));

    //Layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(new QLabel("<H1>Клиент</H1>"));
    pvbxLayout->addWidget(m_ptxtInfo);
    pvbxLayout->addWidget(m_ptxtInput);
    pvbxLayout->addWidget(pcmd);
    setLayout(pvbxLayout);
}

void Client::slotReadyRead()
{
    QDataStream in (m_pTcpSocket);
    in.setVersion(QDataStream::Qt_5_3);

    for(;;) {
        if (!m_nNExtBlockSize) {
            if (m_pTcpSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNExtBlockSize;
        }

        if (m_pTcpSocket->bytesAvailable() < m_nNExtBlockSize) {
                break;
        }

        QTime time;
        QString str;
        in >> time >> str;

        m_ptxtInfo->append(time.toString() + ": " + str);
        m_nNExtBlockSize = 0;
    }
}

void Client::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
            "Error: " + (err == QAbstractSocket::HostNotFoundError ? "Хост не найден." :
                            err == QAbstractSocket::RemoteHostClosedError ? "Хост закрыт." :
                            err == QAbstractSocket::ConnectionRefusedError? "Соединение разорвано." :
                            QString(m_pTcpSocket->errorString())
    );

    m_ptxtInfo->append(strError);
}

void Client::slotSentToServer()
{
    QByteArray arrBlock;
    QDataStream out (&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);  // QT_5_2?
    out << quint16(0) << QTime::currentTime() << m_ptxtInput->text();

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    m_pTcpSocket->write(arrBlock);
    m_ptxtInput->setText("");
}

void Client::slotConnected()
{
    m_ptxtInfo->append("Установка соединения...");
}
