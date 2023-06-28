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
using namespace Enum;

Client::Client(const QString& strHost, int nPort, QWidget* parent /*= nullptr*/)
    : QWidget(parent)
{
    m_pTcpSocket = new QTcpSocket(this);

    m_messageList = new QTextBrowser();
    m_textEdit = new QTextEdit();
    m_pushButton = new QPushButton();
    m_attachFileButton = new QPushButton();
    m_nameList = new QListWidget();
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
    m_messageList->setReadOnly(true);

    m_textEdit->setFixedHeight(100);
    m_pushButton->setIcon(QIcon(":/img/send.png"));
    m_pushButton->setIconSize(QSize(20, 20));
    m_attachFileButton->setIcon(QIcon(":/img/sendImage.png"));
    m_attachFileButton->setIconSize(QSize(20, 20));

    QBoxLayout* textEditeLayout = new QBoxLayout(QBoxLayout::RightToLeft);
    textEditeLayout->addWidget(m_attachFileButton, 0, Qt::AlignRight | Qt::AlignBottom);
    textEditeLayout->setContentsMargins(0,0,0,0);
    m_textEdit->setLayout(textEditeLayout);

    QBoxLayout* messageNamelayout = new QBoxLayout(QBoxLayout::LeftToRight);
    messageNamelayout->setContentsMargins(0,0,0,0);
    messageNamelayout->addWidget(m_splitter);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addLayout(messageNamelayout);
    layout->addWidget(m_textEdit);
    layout->addWidget(m_pushButton);
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
        QImage image;
        in >> time >> str >> image;

        m_nNExtBlockSize = 0;

        this->requestProcessed(str, image);
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
    QString xmlClientName = XmlWriter::ClientName();
    sendToServer(xmlClientName);
}

void Client::sendToServer(QString message, QImage image)
{
    QByteArray arrBlock;
    QDataStream out (&arrBlock, QIODevice::WriteOnly);

    QImage testImage(":/img/user.png"); ///!!!!

    out.setVersion(QDataStream::Qt_5_11);
    out << quint16(0) << QTime::currentTime() << message << image; // !!!!!

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    m_pTcpSocket->write(arrBlock);
}

bool Client::eventFilter(QObject *obj, QEvent *event) //textEdit pressEnter event
{
    if (event->type() == QEvent::Type::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        Qt::KeyboardModifiers modifers = keyEvent->modifiers();
        int key = keyEvent->key();

        if ((modifers == Qt::NoModifier || modifers == Qt::KeypadModifier)
                && (key == 16777220 || key == Qt::Key_Enter)
                ) {
            this->slotSentMessage();
            return true;
        }

        if (modifers == Qt::ControlModifier && key == Qt::Key_V) {
            if (hasImageClip()) {
                sendImage(QApplication::clipboard()->image());
            }
            return false;
        }

        /// debug
        if (key == Qt::Key_1) {
            qDebug() << m_messageList->toHtml();
        }
    }
    return false;
}

void Client::requestProcessed(const QString &message, QImage &image)
{
    QString messageType =  app::XmlReader::getMessageType(message);
    if (messageType == Enum::MessageType::message) {
        this->messageProcessed(message);
    } else if (messageType == Enum::MessageType::namesList) {
        this->namesListProcessed(message);
    } else if (messageType == Enum::MessageType::image) {
        qDebug() << "image";
        this->imageProcessed(message, image);
    }
}

void Client::messageProcessed(const QString &message)
{
    QString htmlMessage = app::XmlReader::getHtmlMessage(message);
    m_messageList->append(htmlMessage);
}

void Client::namesListProcessed(const QString &message)
{
    QVector<QString> namesList = app::XmlReader::getNamesList(message);

    m_nameList->clear();

    m_nameList->setIconSize(QSize(20,20));
    m_nameList->setSelectionMode(QAbstractItemView::NoSelection);
    m_nameList->setViewMode(QListView::ListMode);

    QListWidgetItem* item;

    foreach (QString name, namesList) {
        item = new QListWidgetItem(name, m_nameList);
        item ->setIcon(QIcon(":/img/user.png"));
    }
}

void Client::imageProcessed(const QString &message, QImage &image)
{
    qDebug() <<"message: " << message;
    QString htmlMessage = app::XmlReader::getHtmlMessage(message, MessageType::image);

    QString name = app::XmlReader::getClientName(message, MessageType::image);
    QString time = app::XmlReader::getMessageTime(message, MessageType::image);

    QString filePath = QApplication::applicationDirPath()+QDir::separator() +
            "Data" + QDir::separator() + QDate::currentDate().toString(Qt::SystemLocaleShortDate) + name + time + ".png";

    QFile file;
    if (file.open(QIODevice::WriteOnly)) {
        file.close();
    }

    image.save(filePath);

    int imgHeight = (image.height() < 400)? image.height() : 400;
    int imgWidth = (image.width() < 400)? image.width() : 400;


    htmlMessage += "<p style=\"padding-left: 20px;\"> <img src=\"" + filePath
            + "\"  height=\"" + QString::number(imgHeight) +
            "\" width=\"" + QString::number(imgWidth)+ "\" /> </p>";

    qDebug() << htmlMessage;

    m_messageList->append(htmlMessage);
}

bool Client::hasImageClip()
{
    const QMimeData* mime = QApplication::clipboard()->mimeData();
    return mime->hasImage();
}

void Client::sendImage(QImage image)
{
     QString message = app::XmlWriter::ImageMessage();
     sendToServer(message, image);
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

