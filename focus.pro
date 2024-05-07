QT       += core gui mqtt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17 -Wall -Wfatal-errors

DEFINES += QT_DEPRECATED_WARNINGS

LIBS += -lpaho-mqtt3as -lpaho-mqttpp3 `mysql_config --cflags --libs`

SOURCES += \
    src/adduser.cpp \
    src/auth.cpp \
    src/chatroom.cpp \
    src/chatroomwindow.cpp \
    src/editprofile.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/message.cpp \
    src/mosquitto.cpp \
    src/profile.cpp \
    src/savegroup.cpp \
    src/sql.cpp \
    src/user.cpp

INCLUDEPATH += headers/ /usr/include/mysql/

HEADERS += \
    headers/adduser.h \
    headers/auth.h \
    headers/chatroom.h \
    headers/chatroomwindow.h \
    headers/editprofile.h \
    headers/mainwindow.h \
    headers/message.h \
    headers/mosquitto.h \
    headers/profile.h \
    headers/savegroup.h \
    headers/sql.h \
    headers/user.h

FORMS += \
    ui/adduser.ui \
    ui/auth.ui \
    ui/chatroomwindow.ui \
    ui/editprofile.ui \
    ui/mainwindow.ui \
    ui/profile.ui \
    ui/savegroup.ui

RESOURCES += \
    resources.qrc

TARGET = Focus
OBJECTS_DIR = $$_PRO_FILE_PWD_/bin/
DESTDIR = $$_PRO_FILE_PWD_/bin/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
