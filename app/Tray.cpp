#include "Tray.h"
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QApplication>
#include "app/user.h"
#include "widget.h"
#include "registerform.h"

app::Tray::Tray()
{
    construct();
}

void Tray::construct()
{
    setAccessibleName("Tray");
    m_icon = new QSystemTrayIcon(QIcon(":/img/favicon.ico"));
    m_menu = new QMenu();

    QAction* actShow = new QAction("Открыть чат", m_icon);
    QAction* actSettings = new QAction("Сбросить настройки", m_icon);
    QAction* actQuit = new QAction("Закрыть нахрен", m_icon);

    connect(actShow, SIGNAL(triggered()), this, SLOT(slotShowChat()));
    connect(actSettings, SIGNAL(triggered()), this, SLOT(slotResetSettings()));
    connect(actQuit, SIGNAL(triggered()), this, SLOT(slotQuit()));
    connect(m_icon, SIGNAL(messageClicked()), this, SLOT(slotShowChat()));
    connect(m_icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(slotShowChat()));

    m_menu->addAction(actShow);
    m_menu->addAction(actSettings);
    m_menu->addAction(actQuit);

    m_icon->setContextMenu(m_menu);
    m_icon->setToolTip(QApplication::applicationName());

    m_icon->show();
    connect(m_icon, SIGNAL(messageClicked()), this, SLOT(slotMessageClick()));
}

app::Tray &app::Tray::instance()
{
    static Tray instance;
    return instance;
}

Tray::~Tray()
{

}

void Tray::youHaveNewMessageTip()
{
    m_icon->showMessage(
                QApplication::applicationName(),
                "У вас новое сообщение!",
                QSystemTrayIcon::NoIcon,
                99999999
                );
}

void Tray::emitMessageClick()
{
    emit(m_icon->messageClicked());
}

void app::Tray::slotShowChat()
{
    QWidgetList allWidgets = qApp->topLevelWidgets();
    foreach (auto widget, allWidgets) {
        QString widgetName = widget->accessibleName();
        if (/*widgetName == "Server" ||*/ widgetName == "Client") {
            widget->show();
        }
    }
}

void app::Tray::slotResetSettings()
{
    User::instance().resetSettings();

    QWidgetList allWidgets = qApp->topLevelWidgets();
    foreach (auto widget, allWidgets) {
        QString widgetName = widget->accessibleName();
        if (widgetName == "Server" || widgetName == "Client") {
            delete widget;
        }
        if (widgetName == "MainWidget") {
            widget->show();
        }
    }
}

void app::Tray::slotQuit()
{
    QWidgetList allWidgets = qApp->topLevelWidgets();
    foreach (auto widget, allWidgets) {
        QString widgetName = widget->accessibleName();
        if (widgetName == "Server" || widgetName == "Client") {
            delete widget;
        }
    }
    qApp->quit();
}

void Tray::slotMessageClick()
{
    m_icon->showMessage("", "", QSystemTrayIcon::NoIcon, 0);
}
