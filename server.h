#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTextEdit>
#include <QTcpSocket>

//class QTcpServer;
//class QTextEdit;
//class QTcpSocket;

class Server : public QWidget {
    Q_OBJECT
private:
    QTcpServer* m_ptcpServer;
    QTextEdit* m_ptxt;
    quint16 m_nNExtBlockSize;

private:
    void sendTOClient(QTcpSocket* pSocket, const QString& str);

public:
    Server(int nPort, QWidget* pwgt = 0);

public slots:
    virtual void slotNewConnection();
    void slotReadClient();

};


#endif // SERVER_H
