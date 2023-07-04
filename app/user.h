#ifndef USER_H
#define USER_H

#include <QString>
#include <QSettings>

namespace app {

class User
{
private:
    User();

public:
    static User& instance();

    enum Mode {
        Unknow = 0,
        Client = 1,
        Server = 2
    };

protected:
    QString name;
    const QString filename = "userInfo";
    const qint64 maxNameLength = 20;
    QSettings *m_settings;

public:
    ~User();
    void setName(const QString &name);
    bool checkName();
    QString getName();

    Mode getLastMode();
    void setLastMode(const Mode &value);
    QString getLastIp();
    void setLastIp(const QString &value);
    void resetSettings();

private:
    QString lastModeKey();
    QString lastIpKey();

};

}

using namespace app;
#endif // USER_H
