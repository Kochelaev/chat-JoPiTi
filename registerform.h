#ifndef REGISTERFORM_H
#define REGISTERFORM_H

#include <QtWidgets>

//namespace Ui {
//class RegisterForm;
//}

class RegisterForm : public QWidget
{
    Q_OBJECT

private:
//    Ui::RegisterForm *ui;
    QLabel* m_label;
    QLineEdit* m_input;
    QPushButton* m_okButton;

public:
    explicit RegisterForm(QWidget *parent = 0);
    ~RegisterForm();

signals:
    void signalChangeName();

public slots:
    void on_cancelButton_clicked();

    void on_OkButton_clicked();

protected:
    virtual void closeEvent(QCloseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

};

#endif // REGISTERFORM_H
