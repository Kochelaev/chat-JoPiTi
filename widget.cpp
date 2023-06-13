#include "widget.h"
#include "ui_widget.h"

#include "app/user.h"
#include "registerform.h"
#include "appform.h"
#include "serveripform.h"
#include "server.h"
#include "client.h"

using namespace app;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    ui->setupUi(this);

    QPalette pal;
    pal.setBrush(this->backgroundRole(), QBrush(QPixmap(":/img/image.jpg")));
    this->setPalette(pal);

    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setWindowTitle("Тайный чат");

    this->refreshName();
}

Widget::~Widget()
{
    delete ui;
    qDebug()<<"exit";
}

void Widget::on_ClientButton_clicked()
{
    ServerIpForm* ipForm = new ServerIpForm(this);
    ipForm->show();
}

void Widget::on_serverButton_clicked()
{
    static Server server(Server::serverPort);                 //
    static Client client("localhost", Server::serverPort);    // static bad style?

    client.setWindowTitle("ServeClient");   // delete this later

    server.show();
    client.show();

    this->hide();
}

void Widget::on_changeNameButton_clicked()
{
    RegisterForm* regForm = new RegisterForm(this);
    regForm->show();
}

void Widget::refreshName()
{
    User* user = &User::instance();
    if (user->checkName()) {
        ui->nickName->setText(user->getName());
    }
}
