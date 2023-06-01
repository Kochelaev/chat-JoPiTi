#include "registerform.h"
#include "ui_registerform.h"
#include "widget.h"
#include <QDebug>
#include <QMessageBox>

#include "app/user.h"

using namespace app;

RegisterForm::RegisterForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterForm)
{
    ui->setupUi(this);
    this->setWindowTitle("Тайный чат");
    ui->name->setText(User::instance().getName());
    parent->hide();
}

RegisterForm::~RegisterForm()
{
    qDebug() << "register close";
    delete ui;
}

void RegisterForm::on_OkButton_clicked()
{
    const QString name = ui->name->text();
    if (!name.isEmpty()){
        User::instance().setName(name);

        Widget* parent = dynamic_cast<Widget*>(this->parent());
        parent->refreshName();
        parent->show();
        this->~RegisterForm();
    } else {
        QMessageBox::warning(this, "Внимание!", "Имя не может быть пустым");
    }
}

void RegisterForm::on_cancelButton_clicked()
{
   Widget* parent = dynamic_cast<Widget*>(this->parent());
   if (parent) {
       parent->show();
   }
   this->~RegisterForm();
}
