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

QString Settings::getDefaultFormat()
{
    QString defaultFormat = QSettings().value("defaultFormat").toString();
    if (defaultFormat.isEmpty() || !compatibleFormats.value(getDefaultCodec()).supportedFormats.contains(defaultFormat)) {
        qCritical() << "The default format was invalid: " << defaultFormat;
    }
    return defaultFormat;
}

QStringList Settings::getAvailableCodecs()
{
    QStringList availableCodecs = QSettings().value("availableCodecs").toStringList();
    if (availableCodecs.isEmpty()) {
        qCritical() << "The availableCodecs list is empty";
    }
    return availableCodecs;
}

QStringList Settings::getAvailableEncoders()
{
    QStringList availableEncoders = QSettings().value("availableEncoders").toStringList();
    if (availableEncoders.isEmpty()) {
        qCritical() << "The availableEncoders list is empty";
    }
    return availableEncoders;
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
    QSettings settings;

    if (!settings.value("defaultCodec").toString().isEmpty() &&
        !settings.value("defaultEncoder").toString().isEmpty() &&
        !settings.value("defaultFormat").toString().isEmpty() &&
        !settings.value("availableCodecs").toStringList().isEmpty()) {

        if (settings.value("hardwareId").toByteArray() != getHardwareId()) {
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
    QStringList availableCodecs;
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
            QString codecLine = codec + "@";
            if (defaultCodec.isEmpty()) defaultCodec = codec;
            for (int i=0; i<encoders.length(); i++) {
                if (i == 0) {
                    codecLine.append(encoders.at(i));
                    if (defaultEncoder.isEmpty()) defaultEncoder = encoders.at(i);
                } else {
                    codecLine.append("," + encoders.at(i));
                }
            }
            availableCodecs.append(codecLine);
        }
    }

    settings.setValue("availableCodecs", availableCodecs);

    settings.setValue("defaultCodec", defaultCodec);
    settings.setValue("defaultEncoder", defaultEncoder);
    settings.setValue("defaultFormat", compatibleFormats.value(defaultCodec).supportedFormats.first());

    settings.setValue("hardwareId", getHardwareId());
}

QByteArray Settings::getHardwareId()
{
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(getMotherboardName().toUtf8());
    hash.addData(getCpuName().toUtf8());
    hash.addData(getGpuNames().join("|").toUtf8());
    return hash.result();
}

QStringList Settings::getGpuNames()
{
    QStringList gpuNames;

    #ifdef Q_OS_MAC
    // Nothing because in macos the decoder is always videotoolbox
    #endif

    #ifdef Q_OS_WIN

    #endif

    #ifdef Q_OS_LIN

    #endif

    return gpuNames;
}

QString Settings::getCpuName()
{
    QString cpuName;

    #ifdef Q_OS_MAC
    char buffer[128];
    size_t bufferlen = sizeof(buffer);
    if (sysctlbyname("machdep.cpu.brand_string", &buffer, &bufferlen, nullptr, 0) == 0) {
        cpuName = QString(buffer);
    } else {
        cpuName = "Unknown CPU";
    }
    #endif

    return cpuName;
}

QString Settings::getMotherboardName()
{
    QString motherboardName;

    #ifdef Q_OS_MAC
    size_t len = 0;
    sysctlbyname("hw.model", NULL, &len, NULL, 0);
    if (len) {
        char model[255];
        sysctlbyname("hw.model", &model, &len, NULL, 0);
        motherboardName = model;
    }
    #endif

    return motherboardName;
}

void Settings::qexit(int code)
{
    QCoreApplication::exit(code);
    exit(code);
}
