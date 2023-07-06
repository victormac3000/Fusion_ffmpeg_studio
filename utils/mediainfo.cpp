#include "mediainfo.h"


bool MediaInfo::isVideo(QFile *media)
{
    return getMimeType(media).preferredSuffix() == "mp4";
}

float MediaInfo::getFPS(QFile *video)
{
    QProcess *process = new QProcess;
    QString ffprobePath = getFFProbePath();

    if (ffprobePath.isEmpty()) {
        qWarning() << "FFProbe binary path not found:" << ffprobePath;
    }

    process->setProgram(ffprobePath);
    QStringList params;
    params.append("-v");
    params.append("error");
    params.append("-select_streams");
    params.append("v:0");
    params.append("-of");
    params.append("default=noprint_wrappers=1:nokey=1");
    params.append("-show_entries");
    params.append("stream=r_frame_rate");
    params.append(QDir::toNativeSeparators(video->fileName()));
    process->setArguments(params);
    process->start();
    process->waitForFinished();

    QString rawFps = process->readAllStandardOutput();
    QStringList operands = rawFps.split("/");
    if (operands.size() != 2) {
        return 0.0;
    }
    int a = operands.at(0).toInt();
    int b = operands.at(1).toInt();

    return (float) a/b;
}

QSize MediaInfo::getResolution(QFile *video)
{
    QProcess *process = new QProcess;
    QString ffprobePath = getFFProbePath();

    if (ffprobePath.isEmpty()) {
        qWarning() << "FFProbe binary path not found:" << ffprobePath;
    }

    process->setProgram(ffprobePath);
    QStringList params;
    params.append("-v");
    params.append("error");
    params.append("-select_streams");
    params.append("v:0");
    params.append("-show_entries");
    params.append("stream=width,height");
    params.append("-of");
    params.append("csv=s=x:p=0");
    params.append(QDir::toNativeSeparators(video->fileName()));
    process->setArguments(params);
    process->start();
    process->waitForFinished();

    QString rawFps = process->readAllStandardOutput();
    QStringList operands = rawFps.split("x");
    if (operands.size() != 2) {
        return QSize(-1,-1);
    }
    int a = operands.at(0).toInt();
    int b = operands.at(1).toInt();

    return QSize(a,b);
}

QDateTime MediaInfo::getDate(QFile *media)
{
    QProcess *process = new QProcess;
    QString ffprobePath = getFFProbePath();

    if (ffprobePath.isEmpty()) {
        qWarning() << "FFProbe binary path not found:" << ffprobePath;
    }

    process->setProgram(ffprobePath);
    QStringList params;
    params.append("-v");
    params.append("error");
    params.append("-select_streams");
    params.append("v:0");
    params.append("-show_entries");
    params.append("stream_tags=creation_time");
    params.append("-of");
    params.append("default=noprint_wrappers=1:nokey=1");
    params.append(QDir::toNativeSeparators(media->fileName()));
    process->setArguments(params);
    process->start();
    process->waitForFinished();

    QString rawDate = process->readAllStandardOutput().trimmed();
    QDateTime date = QDateTime::fromString(rawDate, Qt::ISODateWithMs);

    return date;
}

QTime MediaInfo::getLength(QFile *media1)
{
    QProcess *process = new QProcess;
    QString ffprobePath = getFFProbePath();

    if (ffprobePath.isEmpty()) {
        qWarning() << "FFProbe binary path not found:" << ffprobePath;
    }

    process->setProgram(ffprobePath);
    QStringList params;
    params.append("-v");
    params.append("error");
    params.append("-show_entries");
    params.append("format=duration");
    params.append("-of");
    params.append("default=noprint_wrappers=1:nokey=1");
    params.append("-sexagesimal");
    params.append(QDir::toNativeSeparators(media1->fileName()));
    process->setArguments(params);
    process->start();
    process->waitForFinished();

    QString rawLength = process->readAllStandardOutput().trimmed();

    QTime length = QTime::fromString(rawLength);
    return length;
}

bool MediaInfo::isSameLength(QFile *media1, QFile *media2)
{
    QTime time;
    long media1Len = time.msecsTo(getLength(media1)) / 1000;
    long media2Len = time.msecsTo(getLength(media2)) / 1000;

    bool sameLength = media1Len == media2Len;
    if (!sameLength) {
        qWarning() << "The medias are different lengths (" + QString::number(media1Len) + ") vs (" + QString::number(media2Len) + ")";
    }

    return sameLength;
}

QSize MediaInfo::getImageResolution(QFile *image)
{
    QImage img(image->fileName());
    return QSize(img.width(), img.height());
}

bool MediaInfo::isAudio(QFile *media)
{
    return getMimeType(media).preferredSuffix() == "wav";
}

bool MediaInfo::isImage(QFile *media)
{
    return getMimeType(media).preferredSuffix() == "jpg";
}

QMimeType MediaInfo::getMimeType(QFile *media)
{
    QMimeDatabase db;
    return db.mimeTypeForFile(media->fileName(), QMimeDatabase::MatchContent);
}

QMediaMetaData MediaInfo::getMetadata(QFile *media)
{
    QMediaPlayer player;

    player.setSource(QUrl::fromLocalFile(media->fileName()));
    QTimer timer;
    timer.setSingleShot(true);
    QEventLoop loop;
    QEventLoop::connect(&player, &QMediaPlayer::metaDataChanged, &loop, &QEventLoop::quit);
    QEventLoop::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(1000);
    loop.exec();
    bool timeout = !timer.isActive();
    if (timeout) {
        qWarning() << "Timeout loading metadata for media " << media->fileName();
    }

    return  timeout ? QMediaMetaData() : player.metaData();
}

QString MediaInfo::getFFProbePath()
{
    QSettings settings;
    QString resourcesPath;
    QString extension;

    #ifdef Q_OS_MAC
    resourcesPath = ":/Binaries/macOS/ffprobe";
    #endif

    #ifdef Q_OS_WIN
    extension = ".exe";
    resourcesPath = ":/Binaries/windows/ffprobe.exe";
    #endif

    #ifdef Q_OS_Q_OS_LINUX
    resourcesPath = ":/Binaries/linux/ffprobe";
    #endif

    if (resourcesPath.isEmpty()) {
        qWarning() << "No resources found for unknown OS";
        return resourcesPath;
    }

    QDir workingDir(settings.value("workingDir").toString());

    if (!workingDir.exists()) {
        qWarning() << "The working directory does not exist:" <<  workingDir.absolutePath();
        return "";
    }

    if (!workingDir.exists("Binaries")) {
        if (!workingDir.mkdir("Binaries")) {
            qWarning() << "Could not create the binaries directory inside the working directory:" << workingDir.absolutePath();
            return "";
        }
    }

    if (!workingDir.cd("Binaries")) {
        qWarning() << "Could not move to the binaries directory inside the working directory:" << workingDir.absolutePath();
        return "";
    }

    QFile resourcesFile(resourcesPath);
    QString localFilePath(workingDir.absolutePath() + "/ffprobe" + extension);

    if (!QFile::exists(localFilePath)) {
        if (!resourcesFile.copy(localFilePath)) {
            qWarning() << "Could not copy the binary from the resources path:" << resourcesFile.fileName() <<
                "to the binaries path of the working directory:" << workingDir.absolutePath();
            return "";
        }
    }

    if (!QFile(localFilePath).setPermissions(
            QFileDevice::ReadOwner |  QFileDevice::ReadUser |  QFileDevice::ReadGroup | QFileDevice::ReadOther |
            QFileDevice::WriteOwner |
            QFileDevice::ExeOwner |  QFileDevice::ExeUser |  QFileDevice::ExeGroup | QFileDevice::ExeOther)) {
        qWarning() << "Could not set the binary permissions:" << localFilePath;
        return "";
    }

    return localFilePath;
}
