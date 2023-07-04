#include "app/user.h"
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QApplication>

User::User()
{
    if (QFile::exists(QApplication::applicationDirPath()+QDir::separator()+filename)) {
        QFile file(QApplication::applicationDirPath()+QDir::separator()+filename);
        if (file.open(QIODevice::ReadOnly)) {
            this->name = file.read(maxNameLength);
        }
    }
    m_settings = new QSettings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
}

User::~User()
{
}

User& User::instance()
{
    static User instance;
    return instance;
}

void User::setName(const QString &name)
{
    if (!name.isEmpty()) {
        this->name = name;
        QFile file(QApplication::applicationDirPath()+QDir::separator()+filename);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(this->name.toUtf8());
        }
        file.close();
    }
}

bool User::checkName()
{
    return !this->name.isEmpty();
}

QString User::getName()
{
    return this->name;
}

User::Mode User::getLastMode()
{
    return Mode(m_settings->value(lastModeKey()).toInt());
}

void User::setLastMode(const Mode &value)
{
    m_settings->setValue(lastModeKey(), value);
}

QString User::getLastIp()
{
    return QString(m_settings->value(lastIpKey()).toString());
}

void User::setLastIp(const QString &value)
{
    m_settings->setValue(lastIpKey(), value);
}

void User::resetSettings()
{
    m_settings->remove(lastModeKey());
    m_settings->remove(lastIpKey());
}

QString User::lastModeKey()
{
    return QString("last_mode");
}

QString User::lastIpKey()
{
    return QString("last_ip");
}




