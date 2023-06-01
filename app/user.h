#ifndef USER_H
#define USER_H

#include <QString>
#include <QTextDocumentWriter>

namespace app {

class User
{
private:
    User();

public:
    static User& instance();

protected:
    QString name;
    QTextDocumentWriter writer;
    const QString filename = "userInfo";
    const static qint64 maxNameLength = 20;

public:
    ~User();
    void setName(const QString &name);
    bool checkName();
    QString getName();
};

}

using namespace app;
#endif // USER_H
