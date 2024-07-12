#include "settings.h"

void Settings::setup()
{
    setupDefaultProjectPath();
    setupDefaultProjectName();
    setupBinaries();
    setupEncoders();
}

QString Settings::getAppDataPath()
{
    QString appDataPath = QSettings().value("appData").toString();
    if (appDataPath.isEmpty()) {
        setupAppData();
    }
    return QSettings().value("appData").toString();
}

QString Settings::getDefaultProjectName()
{
    QString defaultProjectName = QSettings().value("defaultProjectName").toString();
    if (defaultProjectName.isEmpty()) {
        qWarning() << "defaultProjectName was empty when requested";
    }
    return defaultProjectName;
}

QString Settings::getDefaultProjectPath()
{
    QString defaultProjectPath = QSettings().value("defaultProjectPath").toString();
    if (defaultProjectPath.isEmpty()) {
        qWarning() << "defaultProjectPath was empty when requested";
    }
    return QSettings().value("defaultProjectPath").toString();
}

QString Settings::getFFmpegPath()
{
    QString ffmpegPath = QSettings().value("ffmpegPath").toString();
    if (ffmpegPath.isEmpty()) {
        qCritical() << "ffmpegPath was empty when requested. Renders WILL FAIL";
    }
    return QSettings().value("ffmpegPath").toString();
}

QString Settings::getFFprobePath()
{
    QString ffprobePath = QSettings().value("ffprobePath").toString();
    if (ffprobePath.isEmpty()) {
        qCritical() << "ffprobePath was empty when requested. Renders WILL FAIL";
    }
    return QSettings().value("ffprobePath").toString();
}

void Settings::setupAppData()
{
    QSettings settings;
    QString actualAppData = settings.value("appData").toString();
    QString defaultAppData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir defaultAppDataDir(defaultAppData);

    if (actualAppData.isEmpty() || !QDir(actualAppData).exists() || !QFileInfo(actualAppData).isWritable()) {
        if ((!defaultAppDataDir.exists() && !defaultAppDataDir.mkpath(defaultAppDataDir.absolutePath())) || !QFileInfo(defaultAppData).isWritable()) {
            std::cerr << "Could not create the default appdata folder in the default path. The default appdata path is: "
                      << defaultAppData.toStdString()
                      << std::endl;
            qexit(ERROR_COULD_NOT_CREATE_APPDATA);
        }
        settings.setValue("appData", defaultAppData);
    }
}

void Settings::setupDefaultProjectPath()
{
    QSettings settings;
    QString documentsPath = settings.value("defaultProjectPath").toString();
    QString osDocumentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    if (documentsPath.isEmpty() || !QDir(documentsPath).exists() || !QFileInfo(documentsPath).isWritable()) {
        qDebug() << "Default project path not set, setting default";
        if (!QDir(osDocumentsPath).exists() || !QFileInfo(osDocumentsPath).isWritable()) {
            Dialogs::criticalStart(
                "The os default documents path does not exist or is not writable. The path is: " +
                osDocumentsPath
            );
        }
        settings.setValue("defaultProjectPath", osDocumentsPath);
    }
}

void Settings::setupDefaultProjectName()
{
    QSettings settings;
    QString projectName = settings.value("defaultProjectName").toString();
    if (projectName.isEmpty()) {
        settings.setValue("defaultProjectName", "Project");
    }
}

void Settings::setupBinaries()
{
    QSettings settings;

    if (QFileInfo(settings.value("ffmpegPath").toString()).isExecutable() &&
        QFileInfo(settings.value("ffprobePath").toString()).isExecutable()) {
        return;
    }

    QString ffmpegPath, ffprobePath;

    #ifdef Q_OS_WIN
    setupBinariesWin(&ffmpegPath, &ffprobePath);
    #endif

    #ifdef Q_OS_MAC
    setupBinariesMac(&ffmpegPath, &ffprobePath);
    #endif

    #ifdef Q_OS_LINUX
    setupBinariesLin(&ffmpegPath, &ffprobePath);
    #endif

    if (ffmpegPath.isEmpty() || !QFile(ffmpegPath).exists() || !QFileInfo(ffmpegPath).isExecutable()) {
        Dialogs::criticalStart(
            "FFmpeg not found on system"
        );
    }

    if (ffprobePath.isEmpty() || !QFile(ffprobePath).exists() || !QFileInfo(ffprobePath).isExecutable()) {
        Dialogs::criticalStart(
            "FFprobe not found on system"
        );
    }

    settings.setValue("ffmpegPath", ffmpegPath);
    settings.setValue("ffprobePath", ffprobePath);
}

void Settings::setupBinariesWin(QString* ffmpegPath, QString* ffprobePath)
{
    QDir binariesFolder = QDir(QCoreApplication::applicationDirPath());

    if (!binariesFolder.cd("Binaries")) {
        Dialogs::criticalStart(
            "The binary resources dir does not exist. The path is: " +
            binariesFolder.absolutePath()
        );
    }

    QFileInfo ffmpegInfo(binariesFolder.absolutePath() + "/ffmpeg.exe");
    QFileInfo ffprobeInfo(binariesFolder.absolutePath() + "/ffprobe.exe");

    if (!ffmpegInfo.exists() || !ffmpegInfo.isExecutable()) {
        Dialogs::criticalStart(
            "The ffmpeg binary does not exist or is not executable. The path is: " +
            ffmpegInfo.absolutePath()
        );
    }

    if (!ffprobeInfo.exists() || !ffprobeInfo.isExecutable()) {
        Dialogs::criticalStart(
            "The ffprobe binary does not exist or is not executable. The path is: " +
            ffmpegInfo.absolutePath()
        );
    }

    ffmpegPath->assign(ffmpegInfo.absoluteFilePath());
    ffprobePath->assign(ffprobeInfo.absoluteFilePath());
}

void Settings::setupBinariesLin(QString* ffmpegPath, QString* ffprobePath)
{
    // TODO find included binaries
    ffmpegPath->assign("/usr/bin/ffmpeg");
    ffprobePath->assign("/usr/bin/ffprobe");
}

void Settings::setupBinariesMac(QString* ffmpegPath, QString* ffprobePath)
{
    // TODO find included binaries
    ffmpegPath->assign("/usr/local/bin/ffmpeg");
    ffprobePath->assign("/usr/local/bin/ffprobe");
}

void Settings::setupEncoders()
{
    QProcess p;
    p.start(getFFprobePath(), {"-loglevel", "panic", "-codecs"});


    bool finished = p.waitForFinished(3000);

    if (!finished) {
        Dialogs::criticalStart(
            QString("Getting encoders process timeout") +
            "PROCESS ERROR: " + p.errorString() +
            "EXIT CODE: " + QString::number(p.exitCode()) + "\n" +
            "EXIT STATUS: " + QString::number(p.exitStatus()) + "\n" +
            "STDERR: " + p.readAllStandardError() + "\n" +
            "STDOUT: " + p.readAllStandardOutput()
        );
    }

    QString rawOutput = p.readAllStandardOutput();

    if (rawOutput.isEmpty()) {
        Dialogs::criticalStart(
            "The stdout of the ffprobe codec list is empty"
        );
    }

    QTextStream rawOutputStream(&rawOutput, QIODevice::ReadOnly);
    QStringList supportedEncoders {""};
    int nFound = 0;

    int i=0;
    while (!rawOutputStream.atEnd()) {
        QString line = rawOutputStream.readLine().trimmed();

        // Skip header section (12 first lines)
        if (i < 12) {
            i++;
            continue;
        }

        QRegularExpressionMatch match = QRegularExpression(
            R"(DEV[.DEVASDTILS]{3}\s(\S+)\s+(.*?)\s*(?:\(decoders:\s*([^)]*)\))?\s*(?:\(encoders:\s*([^)]*)\))?$)"
        ).match(line);

        if (!match.hasMatch()) {
            continue;
        }

        QStringList captures = match.capturedTexts();

        if (captures.length() < 3) {
            continue;
        }

        QString name = captures.at(1);
        QString description = captures.at(2);
        QStringList decoders, encoders;

        if (captures.length() > 3) {
            decoders = captures.at(3).split(" ");
        }

        if (captures.length() > 4) {
            encoders = captures.at(4).split(" ");
        }

        qDebug() << name << description << decoders << encoders;

        nFound++;
    }
}

void Settings::qexit(int code)
{
    QCoreApplication::exit(code);
    exit(code);
}
