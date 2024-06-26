#-------------------------------------------------
#
# Project created by QtCreator 2023-05-25T13:54:24
#
#-------------------------------------------------

QT       += core gui
QT       += xml
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#TARGET = chat
#TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp \
    app/user.cpp \
    registerform.cpp \
    server.cpp \
    client.cpp \
    serveripform.cpp \
    app/XmlWriter.cpp \
    enum/MessageType.cpp \
    app/XmlReader.cpp \
    app/Tray.cpp \
    AdminPanel.cpp

HEADERS += \
        widget.h \
    app/user.h \
    registerform.h \
    server.h\
    client.h\
    serveripform.h \
    app/XmlWriter.h \
    enum/MessageType.h \
    app/XmlReader.h \
    app/Tray.h \
    AdminPanel.h

FORMS += \
#    registerform.ui \
    serveripform.ui

RESOURCES += \
    resource.qrc

#DISTFILES +=

DISTFILES += \
    img/oleg.png
