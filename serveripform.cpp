#include "serveripform.h"
#include "ui_serveripform.h"
#include "widget.h"
#include <QDebug>
#include <QMessageBox>

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
    this->setWindowTitle("Тайный чат");
    parent->hide();

    ui->lineEdit->setText(getLastIp());
}

ServerIpForm::~ServerIpForm()
{
    delete ui;
}

void ServerIpForm::on_OkButton_clicked()
{
    bool isCorrectIp= false;
    QString ip = ui->lineEdit->text();
    QHostAddress hostAdd(ip);
    hostAdd.toIPv4Address(&isCorrectIp);

    if ((isCorrectIp && ip.split(".").count() == 4) || ip == "localhost") {
        static Client client(ip, Server::serverPort); //bad style?
        client.show();
        this->deleteLater();
        saveLastIp(ip);
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
