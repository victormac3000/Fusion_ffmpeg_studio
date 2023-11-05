#include "settings.h"

void Settings::setup()
{
    checkCompatible();
    setupAppData();
    setupProjectPath();
    setupBinaries();
}

QString Settings::getAppDataPath()
{   
    setupAppData();
    QSettings settings;
    return settings.value("appData").toString();
}

void Settings::checkCompatible()
{
    bool supported = false;

    #ifdef Q_OS_MAC
        supported = true;
    #endif

    #ifdef Q_OS_WIN
        supported = true;
    #endif

    #ifdef Q_OS_LINUX
        supported = true;
    #endif

    if (!supported) {
        Dialogs::critical("Fusion Ffmpeg Studio only supports Windows, macOS and linux",
                          "OS Not supported");
    }
}

void Settings::setupAppData()
{
    QSettings settings;

    QString actualAppData = settings.value("appData").toString();
    QString defaultAppData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    if (actualAppData.isEmpty() || !QDir(actualAppData).exists()) {
        settings.setValue("appData", defaultAppData);
        if (!QDir(defaultAppData).exists() || !QDir(QDir::rootPath()).mkpath(defaultAppData)) {
            std::cerr << "Could not create the default appdata folder in the default path. The default appdata path is: "
                      << defaultAppData.toStdString() << std::endl;
            exit(ERROR_COULD_NOT_CREATE_APPDATA);
        }
    }
}

void Settings::setupProjectPath()
{
    QSettings settings;

    QString osDocumentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString documentsPath = settings.value("defaultProjectPath").toString();

    if (documentsPath.isEmpty() || !QDir(documentsPath).exists()) {
        qInfo() << "Default project path not set, setting default";
        if (!QDir(osDocumentsPath).exists()) {
            Dialogs::critical("There was an internal error starting the application\nCheck the logs for more information",
                              "The os default documents path does not exist. The path is: " + osDocumentsPath);
        }
        settings.setValue("defaultProjectPath", osDocumentsPath);
    }

}

void Settings::setupBinaries()
{
    QSettings settings;

    #ifdef Q_OS_WIN
    QDir binariesFolder = QDir(QCoreApplication::applicationDirPath());

    if (!binariesFolder.cd("Binaries")) {
        Dialogs::critical("There was an internal error starting the application\nCheck the logs for more information",
                          "The binary resources dir does not exist. The path is: " + binariesFolder.absolutePath());
    }

    QFileInfo ffmpegInfo(binariesFolder.absolutePath() + "/ffmpeg.exe");
    QFileInfo ffprobeInfo(binariesFolder.absolutePath() + "/ffprobe.exe");

    if (!ffmpegInfo.exists() || !ffmpegInfo.isExecutable()) {
        Dialogs::critical("There was an internal error starting the application\nCheck the logs for more information",
                          "The ffmpeg binary does not exist or is not executable. The path is: " + ffmpegInfo.absolutePath());
    }

    if (!ffprobeInfo.exists() || !ffprobeInfo.isExecutable()) {
        Dialogs::critical("There was an internal error starting the application\nCheck the logs for more information",
                          "The ffprobe binary does not exist or is not executable. The path is: " + ffmpegInfo.absolutePath());
    }

    settings.setValue("ffmpeg", ffmpegInfo.absoluteFilePath());
    settings.setValue("ffprobe", ffprobeInfo.absoluteFilePath());
    settings.sync();
    return;
    #endif

    QString ffmpegPath, ffprobePath;

    #ifdef Q_OS_MAC
    ffmpegPath = "/usr/local/bin/ffmpeg";
    ffprobePath = "/usr/local/bin/ffprobe";
    #endif

    #ifdef Q_OS_LINUX
    ffmpegPath = "/usr/bin/ffmpeg";
    ffprobePath = "/usr/bin/ffprobe";
    #endif

    if (ffmpegPath.isEmpty() || !QFile(ffmpegPath).exists() || !QFileInfo(ffmpegPath).isExecutable()) {
        Dialogs::critical(
            "You must install ffmpeg on your machine to use this application",
            "FFmpeg not installed"
        );
    }

    if (ffprobePath.isEmpty() || !QFile(ffprobePath).exists() || !QFileInfo(ffprobePath).isExecutable()) {
        Dialogs::critical(
            "You must install ffprobe on your machine to use this application",
            "FFProbe not installed"
        );
    }

    settings.setValue("ffmpeg", ffmpegPath);
    settings.setValue("ffprobe", ffprobePath);

    settings.sync();
}
