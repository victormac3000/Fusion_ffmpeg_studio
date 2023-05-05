QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# App name
TARGET = 'Fusion ffmpeg studio'

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    models/fformats.cpp \
    models/fsegment.cpp \
    models/fvideo.cpp \
    utils/dialogs.cpp \
    utils/logger.cpp \
    utils/mediainfo.cpp \
    windows/about.cpp \
    windows/mainwindow.cpp \
    panes/editorpane.cpp \
    panes/loadingpane.cpp \
    panes/welcomepane.cpp \
    windows/preferences.cpp \
    worker.cpp

HEADERS += \
    models/fformat.h \
    models/fformats.h \
    models/fsegment.h \
    models/fvideo.h \
    utils/dialogs.h \
    utils/logger.h \
    utils/mediainfo.h \
    windows/about.h \
    windows/mainwindow.h \
    panes/editorpane.h \
    panes/loadingpane.h \
    panes/welcomepane.h \
    windows/preferences.h \
    worker.h

FORMS += \
    windows/about.ui \
    windows/mainwindow.ui \
    panes/editorpane.ui \
    panes/loadingpane.ui \
    panes/welcomepane.ui \
    windows/preferences.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources/Resources.qrc

DISTFILES +=
