#include "widget.h"
#include <QApplication>

#include <QDebug>
#include "app/user.h"
#include <iostream>
#include "registerform.h"
#include "appform.h"

using namespace app;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget* mainWidget = new Widget();

    if (User::instance().checkName()) {
        mainWidget->show();
    } else {
        RegisterForm* regForm = new RegisterForm(mainWidget);
        regForm->show();
    }

    return a.exec();
}
