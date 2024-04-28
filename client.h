#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QtWidgets>

#include "AdminPanel.h"

class QTextEdit;
class QLineEdit;

class Client : public QWidget {
    Q_OBJECT
private:
    QString m_host;
    quint16 m_port;

    QTcpSocket *m_pTcpSocket;
    quint16 m_nNExtBlockSize;

    QTextBrowser *m_messageList;
    QTextEdit *m_textEdit;
    QPushButton *m_pushButton;
    QPushButton *m_attachFileButton;
    QFrame *m_buttonFrame;
    QListWidget *m_nameList;

    QBoxLayout *m_messageLayout;
    QSplitter *m_splitter;

    AdminPanel *m_adminPanel;
    QLabel *oleg = nullptr;
    QPushButton *m_reconnectButton = nullptr;

public:
    Client(const QString &strHost, quint16 nPort, QWidget *parent = 0);
    ~Client();

public:
    void sendToServer(QString message, QImage image = QImage());

protected:
    virtual bool eventFilter(QObject*, QEvent *event) override;

protected:
    void requestProcessed(const QString &message, QImage &image);
    void messageProcessed(const QString &message);
    void namesListProcessed(const QString &message);
    void imageProcessed(const QString &messsage, QImage &image);
    virtual void enterEvent(QEvent *) override;
    void olegProcessed();

private: // Image send methods
    bool hasImageClip();
    bool hasFileClip();
    void sendImage(QImage image);
    void sendFileFromBuffer();

public slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSentMessage();
    void slotConnected();

    void slotAttachFile();
    void slotSendOleg();

    void slotOpenLink(QUrl url);
    void slotReconnect();
};

#endif // CLIENT_H
