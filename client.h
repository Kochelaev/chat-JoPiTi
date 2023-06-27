#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QtWidgets>
#include <QTcpSocket>

class QTextEdit;
class QLineEdit;

class Client : public QWidget
{
    Q_OBJECT
private:
    QTcpSocket* m_pTcpSocket;
    quint16 m_nNExtBlockSize;

    QTextBrowser* m_messageList;
    QTextEdit* m_textEdit;
    QPushButton* m_pushButton;
    QFrame* m_buttonFrame;
    QListWidget* m_nameList;

    QBoxLayout* m_messageLayout;
    QSplitter* m_splitter;

public:
    Client(const QString& strHost, int nPort, QWidget* parent= 0);
    ~Client();

public:
    void sendToServer(QString message);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);

protected:
    void requestProcessed(const QString &message);
    void messageProcessed(const QString &message);
    void namesListProcessed(const QString &message);

private:            // Image send methods
    bool hasImageClip();
    void sendImage();
    void sendImageFromClip();


public slots:
    void slotReadyRead ();
    void slotError(QAbstractSocket::SocketError);
    void slotSentMessage();
    void slotConnected ();
};

#endif // CLIENT_H
