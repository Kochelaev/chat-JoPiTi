#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "serveripform.h"
#include "registerform.h"
#include "server.h"
#include "client.h"
#include "serveripform.h"
#include "StarWindow.h"

//namespace Ui {
//class Widget;
//}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void on_ClientButton_clicked();
    void on_serverButton_clicked();
    void on_changeNameButton_clicked();
    void slotClientChangeIp(const QString &ip);
    void slotChangeName();
    void slotChangeSide();

private:
    Server* m_server;
    Client* m_client;
    ServerIpForm* m_ipForm;
    RegisterForm* m_regForm;

    QPushButton* m_clientButton;
    QPushButton* m_serverButton;
    QPushButton* m_changeNameButton;
    QPushButton* m_darkSideButton;
    QLabel* m_nameTitle;
    QLabel* m_nameLabel;

public:
    void refreshName();

protected:
    virtual bool event(QEvent *event);

private:
    void boostUI();
};

#endif // WIDGET_H
