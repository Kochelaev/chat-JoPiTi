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
    Client(const QString& strHost, int nPort, QWidget* pwgt=0);

private slots:
    void slotReadyRead ();
    void slotError(QAbstractSocket::SocketError);
    void slotSentToServer();
    void slotConnected ();
};

#endif // CLIENT_H
