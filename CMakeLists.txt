cmake_minimum_required(VERSION 3.21)
project(FusionFFmpegStudio VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt 6 modules
find_package(Qt6 6.5 REQUIRED COMPONENTS
    Core
    Gui
    HttpServer
    Widgets
    Sql
    Multimedia
    MultimediaWidgets
    Quick
    QuickWidgets
    WebEngineCore
    QuickControls2
    WebEngineWidgets
    Qml
)
qt_standard_project_setup(REQUIRES 6.5)

# Find ffmpeg packages
if(APPLE OR LINUX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
        libavcodec
        libavformat
        libavutil
        libavfilter
        libavdevice
        libswscale
        libswresample
    )
endif()

set(SOURCES
    main.cpp
)
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/models/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/controllers/*.cpp"
)
list(APPEND SOURCES ${PROJECT_SOURCES})


file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/models/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/controllers/*.h"
)
list(APPEND HEADERS ${PROJECT_HEADERS})

file(GLOB_RECURSE QML_FILES
    CONFIGURE_DEPENDS
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "qml/*.qml"
)
list(APPEND QML_FILES ${PROJECT_QML_FILES})

set(RESOURCES
    Resources/index.qrc
)

qt_add_executable(FusionFFmpegStudio
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)

qt_policy(SET QTP0004 NEW)
qt_add_qml_module(FusionFFmpegStudio
    URI FusionFFmpegStudio
    QML_FILES ${QML_FILES}
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
    Qt6::HttpServer
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
    # Link WIN32 APIs
    target_link_libraries(FusionFFmpegStudio PRIVATE wbemuuid advapi32)
elseif(APPLE OR LINUX)
    # Link ffmpeg
    target_link_libraries(FusionFFmpegStudio PRIVATE PkgConfig::FFMPEG)
endif()




