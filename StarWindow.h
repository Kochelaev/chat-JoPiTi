#ifndef STARWINDOW_H
#define STARWINDOW_H

#include <QWidget>
#include "serveripform.h"
#include "registerform.h"
#include "server.h"
#include "client.h"
#include "serveripform.h"

class StarWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StarWindow(QWidget *parent = 0);
    ~StarWindow();

public slots:
    void on_ClientButton_clicked();

    void on_serverButton_clicked();

    void on_changeNameButton_clicked();

    void slotClientChangeIp(const QString &ip);

    void slotChangeName();

private:
    Server* m_server;
    Client* m_client;
    ServerIpForm* m_ipForm;
    RegisterForm* m_regForm;

    QPushButton* m_clientButton;
    QPushButton* m_serverButton;
    QPushButton* m_changeNameButton;
    QLabel* m_nameTitle;
    QLabel* m_nameLabel;

public:
    void refreshName();

protected:
    virtual bool event(QEvent *event);

private:
    void boostUI();
};

#endif // STARWINDOW_H
