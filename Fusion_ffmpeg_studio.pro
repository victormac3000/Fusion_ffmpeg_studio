QT += core gui xml multimedia multimediawidgets quick quickwidgets quickcontrols2

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

CONFIG += c++17

WEBENGINE_CONFIG += use_proprietary_codecs

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
    models/project.cpp \
    models/renderwork.cpp \
    panes/projectcreator.cpp \
    utils/dialogs.cpp \
    utils/ffmpeg.cpp \
    utils/logger.cpp \
    utils/mediainfo.cpp \
    utils/myqsysinfo.cpp \
    utils/renderer.cpp \
    utils/settings.cpp \
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
    models/project.h \
    models/renderwork.h \
    panes/projectcreator.h \
    utils/dialogs.h \
    utils/exitcodes.h \
    utils/ffmpeg.h \
    utils/logger.h \
    utils/mediainfo.h \
    utils/renderer.h \
    utils/settings.h \
    utils/myqsysinfo.h \
    windows/about.h \
    windows/mainwindow.h \
    panes/editorpane.h \
    panes/loadingpane.h \
    panes/welcomepane.h \
    windows/preferences.h \
    worker.h

FORMS += \
    panes/projectcreator.ui \
    windows/about.ui \
    windows/mainwindow.ui \
    panes/editorpane.ui \
    panes/loadingpane.ui \
    panes/welcomepane.ui \
    windows/preferences.ui

win32 {
    LIBS += -lwbemuuid -ladvapi32
    QMAKE_LFLAGS -= -lwbemuuid -ladvapi32
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
   Resources/Resources.qrc

DISTFILES +=
