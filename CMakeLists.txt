cmake_minimum_required(VERSION 3.21)
project(FusionFFmpegStudio VERSION 1.0 LANGUAGES CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt 6 modules
find_package(Qt6 6.5 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Sql
    Multimedia
    MultimediaWidgets
    Quick
    QuickWidgets
    QuickControls2
    WebEngineWidgets
    Qml
)
qt_standard_project_setup(REQUIRES 6.5)

# Find ffmpeg packages
if(APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
        libavcodec
        libavformat
        libavutil
        libswscale
        libswresample
    )
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

set(SOURCES
    main.cpp
    models/fformats.cpp
    models/fsegment.cpp
    models/fvideo.cpp
    models/project.cpp
    models/renderwork.cpp
    panes/projectcreator.cpp
    panes/projectcreatorfolder.cpp
    panes/projectcreatorsd.cpp

    utils/ffmpeg.cpp
    utils/logger.cpp
    utils/mediainfo.cpp
    utils/myqsysinfo.cpp
    utils/renderer.cpp
    utils/settings.cpp
    utils/dialogs.cpp
    utils/copier.cpp

    windows/mainwindow.cpp
    panes/editorpane.cpp
    panes/loadingpane.cpp
    panes/welcomepane.cpp

    utils/worker.cpp
    controllers/maincontroller.cpp
    controllers/panes/welcomecontroller.cpp
    controllers/panes/new-project/newprojectcontroller.cpp
    controllers/panes/new-project/newprojectfoldercontroller.cpp
    controllers/panes/new-project/newprojectsdcontroller.cpp
)

set(HEADERS
    models/fformat.h
    models/fformats.h
    models/fsegment.h
    models/fvideo.h
    models/loading.h
    models/project.h
    models/renderwork.h
    panes/projectcreator.h
    panes/projectcreatorfolder.h
    panes/projectcreatorsd.h

    utils/exitcodes.h
    utils/ffmpeg.h
    utils/logger.h
    utils/mediainfo.h
    utils/renderer.h
    utils/settings.h
    utils/myqsysinfo.h
    utils/dialogs.h
    utils/copier.h

    windows/mainwindow.h
    panes/editorpane.h
    panes/loadingpane.h
    panes/welcomepane.h

    utils/worker.h
    controllers/maincontroller.h
    controllers/panes/welcomecontroller.h
    controllers/panes/new-project/newprojectcontroller.h
    controllers/panes/new-project/newprojectfoldercontroller.h
    controllers/panes/new-project/newprojectsdcontroller.h
)

set(FORMS
    panes/projectcreator.ui
    panes/projectcreatorfolder.ui
    panes/projectcreatorsd.ui

    windows/mainwindow.ui
    panes/editorpane.ui
    panes/loadingpane.ui
    panes/welcomepane.ui

)

set(QML_FILES
    qml/components/MyButton.qml
    qml/components/MyComboBox.qml
    qml/components/MySlider.qml
    qml/components/MyRangeSlider.qml
    qml/components/Constants.qml
    qml/components/NiceProgressBar.qml
    qml/components/ScrollableTextArea.qml
    qml/components/dialogs/Dialogs.qml
    qml/components/dialogs/DialogContent.qml
    qml/components/dialogs/DialogFooter.qml
    qml/components/dialogs/DialogHeader.qml
    qml/windows/main/Main.qml
    qml/windows/about/About.qml
    qml/windows/about/BuildInfo.qml
    qml/windows/about/Libraries.qml
    qml/windows/preferences/Preferences.qml
    qml/windows/preferences/PreferencesGeneral.qml
    qml/windows/preferences/PreferencesRendering.qml
    qml/panes/Welcome.qml
    qml/panes/new-project/NewProject.qml
    qml/panes/new-project/NewProjectFolder.qml
    qml/panes/new-project/NewProjectSd.qml
)

set(RESOURCES
    resources/index.qrc
)

qt_add_executable(FusionFFmpegStudio
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
    ${RESOURCES}
)

qt_policy(SET QTP0004 NEW)
qt_add_qml_module(FusionFFmpegStudio
    URI FusionFFmpegStudio
    QML_FILES
    ${QML_FILES}
    SOURCES controllers/windows/aboutcontroller.h controllers/windows/aboutcontroller.cpp
    SOURCES controllers/basecontroller.h controllers/basecontroller.cpp
    QML_FILES qml/windows/about/BuildItem.qml
    SOURCES controllers/windows/preferencescontroller.h controllers/windows/preferencescontroller.cpp
    QML_FILES qml/components/dialogs/DialogButton.qml
    SOURCES utils/exceptions/settingsexception.h
    SOURCES  utils/copier.cpp
    SOURCES utils/exceptions/copierexception.h
    QML_FILES qml/components/dialogs/DialogSpinnerContent.qml
    QML_FILES qml/components/Spinner.qml
    QML_FILES qml/components/Utils.qml
    SOURCES utils/exceptions/customexception.h
    SOURCES utils/exceptions/preferencescontrollerexception.h
    SOURCES utils/remover.h utils/remover.cpp
    SOURCES utils/toolbox.h utils/toolbox.cpp
    SOURCES utils/exceptions/toolboxexception.h
    SOURCES utils/exceptions/removerexception.h
    QML_FILES qml/components/MyTextField.qml
)

set_target_properties(FusionFFmpegStudio PROPERTIES
    OUTPUT_NAME "FusionFFmpegStudioApp"
)

target_include_directories(FusionFFmpegStudio PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(FusionFFmpegStudio PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    Qt6::Quick
    Qt6::QuickWidgets
    Qt6::QuickControls2
    Qt6::WebEngineWidgets
    Qt6::Qml
)

# Add defines (like in qmake CONFIG or DEFINES)
target_compile_definitions(FusionFFmpegStudio PRIVATE
    QT_NO_VERSION_TAGGING
    QT_DISABLE_DEPRECATED_BEFORE=0x060000
)

if(WIN32)
    target_link_libraries(FusionFFmpegStudio PRIVATE wbemuuid advapi32)
elseif(APPLE)
    # Link ffmpeg
    target_link_libraries(FusionFFmpegStudio PRIVATE PkgConfig::FFMPEG)
    # Copy macOS embedded resources
    #add_custom_command(TARGET FusionFFmpegStudio POST_BUILD
    #    COMMAND ${CMAKE_COMMAND} -E copy_directory
    #        "${CMAKE_SOURCE_DIR}/resources/Embedded/macOS"
    #        "$<TARGET_BUNDLE_CONTENT_DIR:FusionFFmpegStudio>/MacOS"
    #)
endif()

if(UNIX AND NOT APPLE)
    install(TARGETS FusionFFmpegStudio DESTINATION /opt/FusionFFmpegStudio/bin)
elseif(QNX)
    install(TARGETS FusionFFmpegStudio DESTINATION /tmp/FusionFFmpegStudio/bin)
endif()




