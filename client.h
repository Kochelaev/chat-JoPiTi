#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QWidget>
#include <QTcpSocket>

class QTextEdit;
class QLineEdit;

class Client : public QWidget
{
    Q_OBJECT
private:
    QTcpSocket* m_pTcpSocket;
    QTextEdit* m_ptxtInfo;
    QLineEdit* m_ptxtInput;
    quint16 m_nNExtBlockSize;

public:
    Client(const QString& strHost, int nPort, QWidget* parent= 0);
    ~Client();

public:
    void sendToServer(QString message);

public slots:
    void slotReadyRead ();
    void slotError(QAbstractSocket::SocketError);
    void slotSentMessage();
    void slotConnected ();
};

#endif // CLIENT_H
