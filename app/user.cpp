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
}

User::~User() {}

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

