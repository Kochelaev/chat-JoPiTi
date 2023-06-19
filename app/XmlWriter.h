#ifndef XMLWRITER_H
#define XMLWRITER_H

#include <QString>

namespace app {

    class XmlWriter
    {
    public:
        XmlWriter();

        static QString PrepareClientMessage(const QString &messageText, QString name = "");
        static QString ClientName();
    };

}

using namespace app;

#endif // XMLWRITER_H
