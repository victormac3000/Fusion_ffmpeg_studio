QT       += core gui multimedia multimediawidgets

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
    models/renderwork.cpp \
    panes/items/fqueueitem.cpp \
    panes/items/fvideoitem.cpp \
    utils/dialogs.cpp \
    utils/ffmpeg.cpp \
    utils/logger.cpp \
    utils/mediainfo.cpp \
    utils/renderer.cpp \
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
    models/renderwork.h \
    panes/items/fqueueitem.h \
    panes/items/fvideoitem.h \
    utils/dialogs.h \
    utils/ffmpeg.h \
    utils/logger.h \
    utils/mediainfo.h \
    utils/renderer.h \
    windows/about.h \
    windows/mainwindow.h \
    panes/editorpane.h \
    panes/loadingpane.h \
    panes/welcomepane.h \
    windows/preferences.h \
    worker.h

FORMS += \
    panes/items/fqueueitem.ui \
    panes/items/fvideoitem.ui \
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
