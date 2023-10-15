#include "settings.h"

void Settings::setup()
{
    checkCompatible();

    QSettings settings;

    // Set default project path

    // defaultProjectPath must be documents or other folder

    QString osDocumentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString documentsPath = settings.value("defaultProjectPath").toString();

    if (!QDir(documentsPath).exists()) {
        qInfo() << "Default project path not set, setting default";
        if (!QDir(osDocumentsPath).exists()) {
            Dialogs::critical("There was an internal error starting the application\nCheck the logs for more information",
                        "The os default documents path does not exist. The path is: " + osDocumentsPath);
        }
        settings.setValue("defaultProjectPath", osDocumentsPath);
    }

    // Set the binaries path

    #ifdef Q_OS_WIN
    QDir binariesFolder = QDir(QCoreApplication::applicationDirPath());

    if (!binariesFolder.cd("Binaries")) {
        Dialogs::critical("There was an internal error starting the application\nCheck the logs for more information",
                          "The binary resources dir does not exist. The path is: " + resourcesPath);
    }

    QFileInfo ffmpegInfo(binariesFolder.absolutePath() + "/ffmpeg.exe");
    QFileInfo ffprobeInfo(binariesFolder.absolutePath() + "/ffmpeg.exe");

    if (!ffmpegInfo.exists() || !ffmpegInfo.isExecutable()) {
        Dialogs::critical("There was an internal error starting the application\nCheck the logs for more information",
                          "The ffmpeg binary does not exist or is not executable. The path is: " + ffmpegInfo.absolutePath());
    }

    if (!ffprobeInfo.exists() || !ffprobeInfo.isExecutable()) {
        Dialogs::critical("There was an internal error starting the application\nCheck the logs for more information",
                          "The ffprobe binary does not exist or is not executable. The path is: " + ffmpegInfo.absolutePath());
    }

    settings.setValue("ffmpeg", ffmpegInfo.absolutePath());
    settings.setValue("ffprobe", ffmpegInfo.absolutePath());
    settings.sync();
    return;

    #endif

    QProcess *ffmpegProcess = new QProcess;
    QProcess *ffprobeProcess = new QProcess;

    ffmpegProcess->startDetached("which", {"ffmpeg"});
    ffprobeProcess->startDetached("which", {"ffprobe"});

    ffmpegProcess->waitForStarted();
    ffmpegProcess->waitForFinished();
    ffmpegProcess->waitForReadyRead();
    ffprobeProcess->waitForStarted();
    ffprobeProcess->waitForFinished();
    ffprobeProcess->waitForReadyRead();

    if (ffmpegProcess->exitCode() != 0) {
        Dialogs::critical(
            "You must install ffmpeg on your machine to use this application",
            "FFmpeg not installed"
        );
    }

    if (ffprobeProcess->exitCode() != 0) {
        Dialogs::critical(
            "You must install ffprobe on your machine to use this application",
            "FFProbe not installed"
        );
    }

    QString ffmpegPath = ffmpegProcess->readAllStandardOutput();
    QString ffprobePath = ffprobeProcess->readAllStandardOutput();

    settings.setValue("ffmpeg", ffmpegPath);
    settings.setValue("ffprobe", ffprobePath);

    settings.sync();
}

QString Settings::getAppDataPath()
{
    QSettings settings;

    QString actualAppData = settings.value("appData").toString();
    QString defaultAppData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    if (actualAppData.isEmpty() || !QDir(actualAppData).exists()) {
        settings.setValue("appData", defaultAppData);
        if (!QDir(actualAppData).exists()) {
            Dialogs::critical(
                "There was an internal error starting the application\nCheck the logs for more information",
                "Could not create the default appdata folder in the default path. The default appdata path is: "
                    + defaultAppData
            );
        }
    }

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
