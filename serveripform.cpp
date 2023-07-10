#include "serveripform.h"
#include "ui_serveripform.h"
#include "widget.h"
#include <QMessageBox>
#include "app/user.h"
#include "server.h"
#include "client.h"
#include <QFile>
#include <QDir>
#include <QApplication>

ServerIpForm::ServerIpForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerIpForm)
{
    ui->setupUi(this);
    setAccessibleName("IpForm");
    this->setWindowTitle(QApplication::applicationName());

    ui->lineEdit->setText(getLastIp());

    if (User::instance().getLastMode() == User::Mode::Client
            && !ui->lineEdit->text().isEmpty()) {
        this->on_OkButton_clicked();
    }
}

ServerIpForm::~ServerIpForm()
{
//    delete ui;
}

void ServerIpForm::on_OkButton_clicked()
{
    bool isCorrectIp= false;
    QString ip = ui->lineEdit->text();
    QHostAddress hostAdd(ip);
    hostAdd.toIPv4Address(&isCorrectIp);

    if ((isCorrectIp && ip.split(".").count() == 4) || ip == "localhost") {
        saveLastIp(ip);
        emit signalChangeIp(ip);
    }
    else {
        QMessageBox::about(this, "Внимание", "неверный формат ip адресса");
    }
}

QString ServerIpForm::getLastIp()
{
    QString lastIp = "";
    if (QFile::exists(QApplication::applicationDirPath()+QDir::separator()+lastIpFilename)) {
        QFile file(QApplication::applicationDirPath()+QDir::separator()+lastIpFilename);
        if (file.open(QIODevice::ReadOnly)) {
            lastIp = file.read(100);
        }
    }
    return lastIp;
}

void ServerIpForm::saveLastIp(const QString &ip)
{
    QFile file(QApplication::applicationDirPath()+QDir::separator()+lastIpFilename);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(ip.toUtf8());
    }
    file.close();
}
