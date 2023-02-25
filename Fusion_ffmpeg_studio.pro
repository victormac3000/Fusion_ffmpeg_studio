QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# App name
TARGET = 'Fusion ffmpeg studio'

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    about.cpp \
    errorhandler.cpp \
    main.cpp \
    mainwindow.cpp \
    preferences.cpp \
    worker.cpp

HEADERS += \
    about.h \
    errorhandler.h \
    mainwindow.h \
    preferences.h \
    worker.h

FORMS += \
    about.ui \
    mainwindow.ui \
    preferences.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
