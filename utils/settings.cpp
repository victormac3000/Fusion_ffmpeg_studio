#include "settings.h"

void Settings::setup()
{
    // Check that operating system is compatible
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

    // Copy the binaries to appdata

    QString resourcesPath;
    QString extension;
    QDir appDataDir = QDir(settings.value("appData").toString());

    #ifdef Q_OS_MAC
    resourcesPath = ":/Binaries/macOS/ffprobe";
    #endif

    #ifdef Q_OS_WIN
    resourcesPath = QDir::currentPath() + "/Binaries";
    extension = ".exe";
    #endif

    #ifdef Q_OS_LINUX
    resourcesPath = ":/Binaries/linux/ffprobe";
    #endif

    if (resourcesPath.isEmpty() || !QDir().exists(resourcesPath)) {
        Dialogs::critical("There was an internal error starting the application\nCheck the logs for more information",
                          "The binary resources dir does not exist. The path is: " + resourcesPath);
    }


    if (appDataDir.exists("Binaries") && !QDir(appDataDir.absolutePath() + "/Binaries").removeRecursively()) {
        Dialogs::critical("There was an internal error starting the application\nCheck the logs for more information",
                          "Could not remove the appdata binaries. The appdata path is: " + appDataDir.absolutePath());
    }

    if (!appDataDir.mkdir("Binaries")) {
        Dialogs::critical("There was an internal error starting the application\nCheck the logs for more information",
                          "Could not create the binaries directory inside the appData directory: " + appDataDir.absolutePath());
    }

    if (!appDataDir.cd("Binaries")) {
        Dialogs::critical("There was an internal error starting the application\nCheck the logs for more information",
            "Could not find the Binaries directory inside the appData directory: " + appDataDir.absolutePath());
    }

    QString ffmpegSourcePath = resourcesPath + "/ffmpeg" + extension;
    QString ffprobeSourcePath = resourcesPath + "/ffprobe" + extension;
    QString ffmpegDestPath = appDataDir.absolutePath() + "/ffmpeg" + extension;
    QString ffprobeDestPath = appDataDir.absolutePath() + "/ffprobe" + extension;

    if (ffmpegSourcePath.isEmpty() || ffprobeSourcePath.isEmpty() ||
        !QFile::exists(ffmpegSourcePath) || !QFile::exists(ffprobeSourcePath)) {
        Dialogs::critical("There was an internal error starting the application\nCheck the logs for more information",
                          "Could not find the required binaries in folder: " + resourcesPath);
    }

    if (!QFile::copy(ffmpegSourcePath, ffmpegDestPath) || !QFile::copy(ffprobeSourcePath, ffprobeDestPath)) {
        Dialogs::critical("There was an internal error starting the application\nCheck the logs for more information",
                          "Could not copy the required binaries from the resources path: " + resourcesPath
                          + " to the appData path: " + appDataDir.absolutePath());
    }

    if (!QFile(ffmpegDestPath).setPermissions(
            QFileDevice::ReadOwner |  QFileDevice::ReadUser |  QFileDevice::ReadGroup | QFileDevice::ReadOther |
            QFileDevice::WriteOwner |
            QFileDevice::ExeOwner |  QFileDevice::ExeUser |  QFileDevice::ExeGroup | QFileDevice::ExeOther) ||
        !QFile(ffprobeDestPath).setPermissions(
            QFileDevice::ReadOwner |  QFileDevice::ReadUser |  QFileDevice::ReadGroup | QFileDevice::ReadOther |
            QFileDevice::WriteOwner |
            QFileDevice::ExeOwner |  QFileDevice::ExeUser |  QFileDevice::ExeGroup | QFileDevice::ExeOther)) {
        Dialogs::critical("There was an internal error starting the application\nCheck the logs for more information",
                          "Could not set the permissions to the required binaries");
    }
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
