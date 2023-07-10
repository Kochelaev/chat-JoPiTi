#ifndef TRAY_H
#define TRAY_H

#include <QSystemTrayIcon>
#include <QLabel>
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

    public slots:
        void slotShowChat();
        void slotResetSettings();
        void slotQuit();
    };

}
#endif // TRAY_H
