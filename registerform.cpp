#include "registerform.h"
//#include "ui_registerform.h"
#include "widget.h"
#include <QMessageBox>

#include "app/user.h"

using namespace app;

RegisterForm::RegisterForm(QWidget *parent) :
    QWidget(parent)
{
    this->setWindowTitle(QApplication::applicationName());

    m_input = new QLineEdit();
    m_okButton = new QPushButton("Ok");
    m_label = new QLabel("Введите имя:");

    m_input = new QLineEdit();
    if (User::instance().checkName()) {
        m_input->setText(User::instance().getName());
    }
    m_okButton = new QPushButton("Ok");

    connect(m_okButton, SIGNAL(clicked()), this, SLOT(on_OkButton_clicked()));

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(m_label);
    layout->addWidget(m_input);
    layout->addWidget(m_okButton);
    this->setLayout(layout);
    this->setAutoFillBackground(true);
    this->resize(200, 120);

}

RegisterForm::~RegisterForm()
{
}

void RegisterForm::on_OkButton_clicked()
{

    const QString name = m_input->text();
    if (name.count() > User::instance().getMaxNameLength()) {
        QMessageBox::warning(this, "Внимание!", "Имя слишком длинное");
    }
    else if (!name.isEmpty()){
        User::instance().setName(name);
        emit(signalChangeName());
    } else {
        QMessageBox::warning(this, "Внимание!", "Имя не может быть пустым");
    }
}

void RegisterForm::closeEvent(QCloseEvent *event)
{
    emit(signalChangeName());
}

void RegisterForm::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter || event->key() == 16777220) {
        on_OkButton_clicked();
    }
}

void RegisterForm::on_cancelButton_clicked()
{
}
