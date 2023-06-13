#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTextEdit>
#include <QTcpSocket>

#include <QVector>

class Server : public QWidget {
    Q_OBJECT
private:
    QTcpServer* m_ptcpServer;
    QTextEdit* m_ptxt;
    quint16 m_nNExtBlockSize;
    QVector<QTcpSocket*> activeConnections;

private:
    void sendTOClient(QTcpSocket* pSocket, const QString& str);
    void sendToAll(const QString& str);

public:
    Server(int nPort, QWidget* pwgt = 0);
    static const int serverPort = 8005;

public slots:
    virtual void slotNewConnection();
    void slotReadClient();
    void slotClientDisconnect();
};


#endif // SERVER_H
