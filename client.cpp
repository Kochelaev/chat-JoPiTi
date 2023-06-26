#include "client.h"
#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>
#include <QPushButton>
#include "app/XmlWriter.h"
#include "app/XmlReader.h"
#include "enum/MessageType.h"

using namespace app;

Client::Client(const QString& strHost, int nPort, QWidget* parent /*= nullptr*/)
    : QWidget(parent)
{
    m_pTcpSocket = new QTcpSocket(this);

    m_messageList = new QTextEdit();
    m_textEdit = new QTextEdit();
    m_pushButton = new QPushButton();
    m_nameList = new QTextEdit();
    m_splitter = new QSplitter(Qt::Horizontal);

    m_pTcpSocket->connectToHost(strHost, nPort);
    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));

    connect(m_pushButton, SIGNAL(clicked()), SLOT(slotSentMessage()));
    m_textEdit->installEventFilter(this);

    m_splitter->addWidget(m_messageList);
    m_splitter->addWidget(m_nameList);
    QList<int> sizes;
    sizes.push_front(200);
    sizes.push_back(50);
    m_splitter->setSizes(sizes);
    m_nameList->setReadOnly(true);
    m_messageList->setReadOnly(true);

    m_textEdit->setFixedHeight(100);
    m_pushButton->setIcon(QIcon(":/img/img/send.png"));
    m_pushButton->setIconSize(QSize(20, 20));

    QBoxLayout* textEditeLayout = new QBoxLayout(QBoxLayout::RightToLeft);
    textEditeLayout->addWidget(m_pushButton, 0, Qt::AlignRight | Qt::AlignBottom);
    textEditeLayout->setContentsMargins(0,0,0,0);
    m_textEdit->setLayout(textEditeLayout);

    QBoxLayout* messageNamelayout = new QBoxLayout(QBoxLayout::LeftToRight);
    messageNamelayout->setContentsMargins(0,0,0,0);
    messageNamelayout->addWidget(m_splitter);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addLayout(messageNamelayout);
    layout->addWidget(m_textEdit);
    setLayout(layout);

}

Client::~Client()
{
    qDebug() << "Client Destruct";
    m_pTcpSocket->disconnectFromHost();
}

void Client::slotReadyRead()
{
    QDataStream in (m_pTcpSocket);
    in.setVersion(QDataStream::Qt_5_11);

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

        m_nNExtBlockSize = 0;

//        m_messageList->append(time.toString() + ": " + str);

        this->requestProcessed(str);
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

    m_messageList->append(strError);
}

void Client::slotConnected()
{
    m_messageList->append("Установка соединения...");
    QString xmlClientName = XmlWriter::ClientName();
    sendToServer(xmlClientName);
}

void Client::sendToServer(QString message)
{
    QByteArray arrBlock;
    QDataStream out (&arrBlock, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_5_11);
    out << quint16(0) << QTime::currentTime() << message;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    m_pTcpSocket->write(arrBlock);
}

bool Client::eventFilter(QObject *obj, QEvent *event) //textEdit pressEnter event
{
    if (event->type() == QEvent::Type::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        if (keyEvent->key() == 16777220 && keyEvent->modifiers() == Qt::NoModifier) {
            this->slotSentMessage();
            return true;
        }
    }
    return false;
}

void Client::requestProcessed(const QString &message)
{
    QString messageType =  app::XmlReader::getMessageType(message);
    if (messageType == Enum::MessageType::message) {
        this->messageProcessed(message);
    } else if (messageType == Enum::MessageType::namesList) {
        this->namesListProcessed(message);
    }
}

void Client::messageProcessed(const QString &message)
{

}

void Client::namesListProcessed(const QString &message)
{
    QVector<QString> namesList = app::XmlReader::getNamesList(message);
}

void Client::slotSentMessage()
{
    if (m_textEdit->toPlainText().isEmpty()) {
        return;
    }

    QString message = XmlWriter::PrepareClientMessage(m_textEdit->toPlainText());
    m_textEdit->setText("");

    sendToServer(message);
}

