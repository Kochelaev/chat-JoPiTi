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
    QVector<QTcpSocket*> activeConnections;
    QMap<QTcpSocket*, QString> clientNames;

private:
    void sendTOClient(QTcpSocket* pSocket, const QString& str, QImage image = QImage());
    void sendToAll(const QString& str, QImage image = QImage());
    void requestProcessing(const QString& in, QTcpSocket* sender, QImage image = QImage());
    void processMessage(const QString& in, QTcpSocket* sender);
    void processSendClientName(const QString& in, QTcpSocket* sender);
    void processImage(QTcpSocket *sender, QImage &image);
    void olegProcessed(const QString& in);

    void refreshNameList();
    void sendNameList();

public:
    Server(int nPort, QWidget* pwgt = 0);
    ~Server();
    static const int serverPort = 8005;

public slots:
    virtual void slotNewConnection();
    void slotReadClient();
    void slotClientDisconnect();
};


#endif // SERVER_H
