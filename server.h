#ifndef SERVER_H
#define SERVER_H

#include <QtWidgets>
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
    QTextEdit* m_nameList;
    QVector<QTcpSocket*> activeConnections;  // по сути, можно отказаться в пользу Qmap
    QMap<QTcpSocket*, QString> clientNames;

private:
    void sendTOClient(QTcpSocket* pSocket, const QString& str);
    void sendToAll(const QString& str);
    void requestProcessing(const QString& in, QTcpSocket* sender);
    void processMessage(const QString& in, QTcpSocket* sender);
    void processSendClientName(const QString& in, QTcpSocket* sender);

    void refreshNameList();

public:
    Server(int nPort, QWidget* pwgt = 0);
    static const int serverPort = 8005;

public slots:
    virtual void slotNewConnection();
    void slotReadClient();
    void slotClientDisconnect();
};


#endif // SERVER_H
