#include "widget.h"
#include <QApplication>

#include <QDebug>
#include "app/user.h"
#include <iostream>
#include "registerform.h"
#include "appform.h"
#include "serveripform.h"
#include "client.h"
#include "server.h"

using namespace app;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget mainWidget;

    if (User::instance().checkName()) {
        mainWidget.show();
    } else {
        RegisterForm* regForm = new RegisterForm(&mainWidget);
        regForm->show();
    }

    return a.exec();
}
