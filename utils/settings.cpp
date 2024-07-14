#include "settings.h"

QMap<QString,FormatInfo> Settings::compatibleFormats = {
    {"h264", {{"mp4", "mkv", "avi", "mov"}, "Most compatible"}},
    {"hevc", {{"mp4", "mkv", "mov"}, "Most efficient"}},
    {"mpeg4", {{"mp4", "mkv", "avi", "mov"}, "Older codec"}},
    {"prores", {{"mov", "mxf"}, "Best for editing"}}
};

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

QString Settings::getDefaultCodec()
{
    QString defaultCodec = QSettings().value("defaultCodec").toString();
    if (defaultCodec.isEmpty() || !compatibleFormats.contains(defaultCodec)) {
        qCritical() << "The default codec was invalid: " << defaultCodec;
    }
    return defaultCodec;
}

QString Settings::getDefaultEncoder()
{
    QString defaultEncoder = QSettings().value("defaultEncoder").toString();
    if (defaultEncoder.isEmpty()) {
        qCritical() << "The default encoder was invalid: " << defaultEncoder;
    }
    return defaultEncoder;
}

void Settings::resetDefaultFormat()
{
    QSettings settings;
    settings.setValue("defaultFormat", compatibleFormats.value(getDefaultCodec()).supportedFormats.first());
}

QString Settings::getDefaultFormat()
{
    QString defaultFormat = QSettings().value("defaultFormat").toString();
    if (defaultFormat.isEmpty() || !compatibleFormats.value(getDefaultCodec()).supportedFormats.contains(defaultFormat)) {
        qCritical() << "The default format was invalid: " << defaultFormat;
    }
    return defaultFormat;
}

void Settings::setDefaultCodec(QString defaultCodec)
{
    QSettings settings;
    if (defaultCodec.isEmpty()) {
        qWarning() << "The default codec to set is empty";
        return;
    }
    if (!getAvailableCodecs().contains(defaultCodec)) {
        qWarning() << "The provided codec is not available";
        return;
    }
    settings.setValue("defaultCodec", defaultCodec);
}

void Settings::setDefaultEncoder(QString defaultEncoder)
{
    QSettings settings;
    QString defaultCodec = settings.value("defaultCodec").toString();
    if (defaultEncoder.isEmpty()) {
        qWarning() << "The default encoder to set is empty";
        return;
    }
    if (!getAvailableEncoders(defaultCodec).contains(defaultEncoder)) {
        qWarning() << "The provided encoder is not available";
        return;
    }
    settings.setValue("defaultEncoder", defaultEncoder);
}

void Settings::setDefaultFormat(QString defaultFormat)
{
    QSettings settings;
    QString defaultCodec = settings.value("defaultCodec").toString();
    if (defaultFormat.isEmpty()) {
        qWarning() << "The default format to set is empty";
        return;
    }
    if (!getAvailableFormats(defaultCodec).contains(defaultFormat)) {
        qWarning() << "The provided format is not available";
        return;
    }
    settings.setValue("defaultFormat", defaultFormat);
}

QStringList Settings::getAvailableCodecs()
{
    QStringList availableCodecs = QSettings().value("availableCodecs").toStringList();
    if (availableCodecs.empty()) {
        qCritical() << "The available codecs list is empty";
    }
    return availableCodecs;
}

QStringList Settings::getAvailableEncoders(QString codec)
{
    QStringList availableEncoders = QSettings().value("availableEncoders").toStringList();
    QStringList availableEncodersReturn;

    if (availableEncoders.empty()) {
        qCritical() << "The available encoders list is empty";
    }

    for (const QString &availableEncoder: availableEncoders) {
        QStringList availableEncoderSplitted = availableEncoder.split("@");
        if (availableEncoderSplitted.length() != 2) {
            qCritical() << "Found an encoder with an invalid format";
            continue;
        }
        if (availableEncoderSplitted.last() == codec) {
            availableEncodersReturn.append(availableEncoderSplitted.first());
        }
    }

    if (availableEncodersReturn.empty()) {
        qCritical() << "Could not find the encoders for the codec " << codec;
    }

    return availableEncodersReturn;
}

QStringList Settings::getAvailableFormats(QString codec)
{
    return compatibleFormats.value(codec).supportedFormats;
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

    ffmpegPath = ffmpegInfo.absoluteFilePath();
    ffprobePath = ffprobeInfo.absoluteFilePath();
    #endif

    #ifdef Q_OS_MAC
    // TODO find included binaries
    ffmpegPath = "/usr/local/bin/ffmpeg";
    ffprobePath = "/usr/local/bin/ffprobe";
    #endif

    #ifdef Q_OS_LINUX
    // TODO find included binaries
    ffmpegPath = "/usr/bin/ffmpeg";
    ffprobePath = "/usr/bin/ffprobe";
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

void Settings::setupEncoders()
{
    QSettings settings;
    QByteArray hardwareId = MyQSysInfo::hardwareId();

    if (!settings.value("defaultCodec").toString().isEmpty() &&
        !settings.value("defaultEncoder").toString().isEmpty() &&
        !settings.value("defaultFormat").toString().isEmpty() &&
        !settings.value("availableCodecs").toStringList().isEmpty() &&
        !settings.value("availableEncoders").toStringList().isEmpty()) {

        if (settings.value("hardwareId").toByteArray() != hardwareId) {
            Dialogs::ok("Looks like you changed some hardware on your computer. The encoding settings will be reset.");
        } else {
            return;
        }
    }

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
    QStringList availableCodecs, availableEncoders;
    QString defaultCodec, defaultEncoder;

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

        QString codec = captures.at(1);
        QStringList encoders;

        if (captures.length() > 4) {
            encoders = captures.at(4).split(" ");
        }


        if (!encoders.isEmpty() && compatibleFormats.contains(codec)) {
            if (defaultCodec.isEmpty()) defaultCodec = codec;
            if (defaultEncoder.isEmpty()) defaultEncoder = encoders.first();
            availableCodecs.append(codec);
            for (const QString &encoder: encoders) {
                availableEncoders.append(encoder + "@" + codec);
            }
        }
    }

    settings.setValue("availableCodecs", availableCodecs);
    settings.setValue("availableEncoders", availableEncoders);

    settings.setValue("defaultCodec", defaultCodec);
    settings.setValue("defaultEncoder", defaultEncoder);
    settings.setValue("defaultFormat", compatibleFormats.value(defaultCodec).supportedFormats.first());

    settings.setValue("hardwareId", hardwareId);
}

void Settings::qexit(int code)
{
    QCoreApplication::exit(code);
    exit(code);
}
