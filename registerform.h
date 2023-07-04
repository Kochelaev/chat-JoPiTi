#ifndef REGISTERFORM_H
#define REGISTERFORM_H

#include <QDialog>

namespace Ui {
class RegisterForm;
}

class RegisterForm : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterForm(QWidget *parent = 0);
    ~RegisterForm();

private slots:
    void on_cancelButton_clicked();

    void on_OkButton_clicked();

private:
    Ui::RegisterForm *ui;

protected:
    virtual bool event(QEvent *event);
};

#endif // REGISTERFORM_H
