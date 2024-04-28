#ifndef TRAY_H
#define TRAY_H

#include <QSystemTrayIcon>
#include <QLabel>
#include <QMenu>

namespace app {

    class Tray : public QWidget
    {
        Q_OBJECT

    private:
        QSystemTrayIcon* m_icon;
        QMenu* m_menu;
        Tray();

    public:
        static Tray& instance();
        void construct();
        ~Tray();
        void youHaveNewMessageTip();
        void emitMessageClick();

    public slots:
        void slotShowChat();
        void slotResetSettings();
        void slotQuit();
        void slotMessageClick();
    };

}
#endif // TRAY_H

