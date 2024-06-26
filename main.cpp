#include "widget.h"
#include <QApplication>
#include "app/user.h"
#include "registerform.h"
#include "appform.h"
#include "serveripform.h"
#include "client.h"
#include "server.h"
#include <app/user.h>
#include <QSystemTrayIcon>
#include "app/Tray.h"
#include <QDebug>


using namespace app;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString cssPath =
            User::instance().isDarkSide()?
            ":/css/darkSide.css" : ":/css/style.css";

    QFile styleFile(cssPath);
    styleFile.open(QFile::ReadOnly);
    app.setStyleSheet(QLatin1String(styleFile.readAll()));

    QCoreApplication::setOrganizationName("Chat");
    QCoreApplication::setApplicationName("Chat-joPiTi");
    QCoreApplication::setApplicationVersion("0.1");
    QApplication::setWindowIcon(QIcon(":/img/favicon.ico"));

    QApplication::setQuitOnLastWindowClosed(false);

    new Widget();

    Tray::instance();

    return app.exec();
}
