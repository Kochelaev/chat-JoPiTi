#include "widget.h"
#include "ui_widget.h"

#include "app/user.h"
#include "registerform.h"
#include "appform.h"

using namespace app;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setWindowTitle("Тайный чат");

    this->refreshName();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_ClientButton_clicked()
{

}

void Widget::on_serverButton_clicked()
{
//    AppForm* appForm = new AppForm();
//    this->~Widget();
//    appForm->show();
}

void Widget::on_changeNameButton_clicked()
{
    RegisterForm* registerForm = new RegisterForm(this);
    registerForm->show();
}

void Widget::refreshName()
{
    User* user = &User::instance();
    if (user->checkName()) {
        ui->nickName->setText(user->getName());
    }
}

