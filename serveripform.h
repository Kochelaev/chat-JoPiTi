#ifndef SERVERIPFORM_H
#define SERVERIPFORM_H

#include <QDialog>
#include <QString>

namespace Ui {
class ServerIpForm;
}

class ServerIpForm : public QDialog
{
    Q_OBJECT

public:
    explicit ServerIpForm(QWidget *parent = 0);
    ~ServerIpForm();

private slots:
    void on_OkButton_clicked();

//    void on_ServerIpForm_destroyed();

private:
    Ui::ServerIpForm *ui;
    QString lastIpFilename = "lastIp";

    QString getLastIp();
    void saveLastIp(const QString &ip);
};

#endif // SERVERIPFORM_H
