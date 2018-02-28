#-------------------------------------------------
#
# Project created by QtCreator 2016-11-02T17:08:26
#
#-------------------------------------------------

QT       += core gui network sql websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Server
TEMPLATE = app


SOURCES +=\
        serverwindow.cpp \
    mythread.cpp \
    myserver.cpp \
    database.cpp \
    changeadminpassword.cpp

HEADERS  += serverwindow.h \
    mythread.h \
    myserver.h \
    database.h \
    changeadminpassword.h

FORMS    += serverwindow.ui \
    changeadminpassword.ui

DISTFILES += \
    chat.sqlite \
    Chat.sqliteold

RESOURCES += \
    server.qrc
