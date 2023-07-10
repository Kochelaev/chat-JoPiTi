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
#include "app/Tray.h"

using namespace app;
using namespace Enum;

Client::Client(const QString& strHost, int nPort, QWidget* parent /*= nullptr*/)
    : QWidget(parent)
{
    setAccessibleName("Client");

    m_nNExtBlockSize = 0;
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
    connect(m_attachFileButton, SIGNAL(clicked()), SLOT(slotAttachFile()));
    m_textEdit->installEventFilter(this);

    m_splitter->addWidget(m_messageList);
    m_splitter->addWidget(m_nameList);
    QList<int> sizes;
    sizes.push_front(200);
    sizes.push_back(50);
    m_splitter->setSizes(sizes);
    m_messageList->setReadOnly(true);
    m_messageList->setOpenLinks(false);

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

    connect(m_messageList, SIGNAL(anchorClicked(QUrl)), this, SLOT(slotOpenLink(QUrl)));

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

void Client::slotAttachFile()
{
    QString filename = QFileDialog::getOpenFileName(0, "Выберите изображение", "", "*.png *.jpg *.jpeg *.bmp").toUtf8();

    if (!filename.isEmpty()) {
        this->sendImage(QImage(filename));
    }
}

void Client::slotOpenLink(QUrl url)
{
    QImage img(url.toString());

    if (img.isNull()) {
        qDebug() << "Img is empty. return";
        return;
    }

    QWidget* imgWidget = new QWidget();
    QPalette pal;
    pal.setBrush(imgWidget->backgroundRole(), QBrush(img));

    imgWidget->setPalette(pal);
    imgWidget->setAutoFillBackground(true);

    imgWidget->resize(img.size());
    imgWidget->show();
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
                return true;
            }
            if (hasFileClip()){
                sendFileFromBuffer();
                return true;
            }
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
        this->imageProcessed(message, image);
    }
}

void Client::messageProcessed(const QString &message)
{
    QString htmlMessage = app::XmlReader::getHtmlMessage(message);
    m_messageList->append(htmlMessage);
    if (!this->isActiveWindow()) {
        QWidgetList allWidgets = qApp->topLevelWidgets();
        foreach (auto widget, allWidgets) {
            if (widget->accessibleName() == "Tray") {
                Tray* tray = qobject_cast<Tray*>(widget);
                tray->youHaveNewMessageTip();
            }
        }
    }

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
    QString htmlMessage = app::XmlReader::getHtmlMessage(message, MessageType::image);

    QString name = app::XmlReader::getClientName(message, MessageType::image);
    QString time = app::XmlReader::getMessageTime(message, MessageType::image);

    QString filePath = QApplication::applicationDirPath()+QDir::separator() +
            "Data" + QDir::separator() + QDate::currentDate().toString(Qt::SystemLocaleShortDate)
            + name + time + ".png";

    QFile file;
    if (file.open(QIODevice::WriteOnly)) {
        file.close();
    }
    //исправь, объедини с filePath
    QString dirname = QApplication::applicationDirPath() + QDir::separator() + "Data";
    QDir fileDir (dirname);
    if(!fileDir.exists(dirname)) {
        fileDir.mkdir(dirname);
    }

    image.save(filePath, "png");

    int imgHeight = image.height();
    int imgWidth = image.width();
    int maxSize = std::max(imgHeight, imgWidth);

    if (maxSize > 400) {
        double imgScale = maxSize / 400;
        imgHeight = imgHeight / imgScale;
        imgWidth = imgWidth / imgScale;
    }

    htmlMessage += "<a href=\"" + filePath + "\"><p style=\"padding-left: 20px;\"> <img src=\"" + filePath
            + "\"  height=\"" + QString::number(imgHeight) +
            "\" width=\"" + QString::number(imgWidth)+ "\"/></p></a>";

    m_messageList->append(htmlMessage);
}

void Client::enterEvent(QEvent *event)
{
    Tray::instance().emitMessageClick();
}

bool Client::hasImageClip()
{
    const QMimeData* mime = QApplication::clipboard()->mimeData();
    return mime->hasImage();
}

bool Client::hasFileClip()
{
    const QMimeData* mime = QApplication::clipboard()->mimeData();
    return mime->hasUrls();
}

void Client::sendImage(QImage image)
{
     QString message = app::XmlWriter::ImageMessage();
     sendToServer(message, image);
}

void Client::sendFileFromBuffer()
{
    QString linkString = QApplication::clipboard()->text();
    linkString.replace("file://", "");
    QImage img(linkString);
    if (img.isNull()) {
        return;
    }
    sendImage(img);
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

