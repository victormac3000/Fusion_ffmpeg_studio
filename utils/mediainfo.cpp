#include "mediainfo.h"
#include "utils/settings.h"


bool MediaInfo::isVideo(QFile *media)
{
    return getMimeType(media).preferredSuffix() == "mp4";
}

VideoInfo MediaInfo::getVideoInfo(QFile *video)
{
    QElapsedTimer tmr;
    tmr.start();

    QProcess *process = new QProcess;
    QString ffprobePath = Settings::getFFprobePath();

    process->setProgram(ffprobePath);
    QStringList params;
    params.append("-v");
    params.append("quiet");
    params.append("-select_streams");
    params.append("v:0");
    params.append("-show_entries");
    params.append("stream=width,height,avg_frame_rate:stream_tags=creation_time:format=duration");
    params.append("-of");
    params.append("json");
    params.append(QDir::toNativeSeparators(video->fileName()));
    process->setArguments(params);
    process->start();
    process->waitForFinished();

    if (process->exitCode() != 0) {
        qWarning() << "There was an error executing ffprobe" << ffprobePath;
        return VideoInfo();
    }

    QString normalOut = process->readAllStandardOutput();
    QString errorOut = process->readAllStandardError();

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(normalOut.toUtf8(), &jsonError);
    if (jsonError.error != jsonError.NoError) {
        qWarning() << "There was an error parsing the JSON from ffprobe: " << errorOut <<  " The JSON was: " << normalOut;
        return VideoInfo();
    }

    QString rawLength = doc.object().value("format").toObject().value("duration").toString();
    QJsonArray streams = doc.object().value("streams").toArray();
    if (streams.size() < 1) {
        qWarning() << "FFprobe did not return any streams for the media " << video->fileName();
        return VideoInfo();
    }
    int height = streams.at(0).toObject().value("height").toInt(0);
    int width = streams.at(0).toObject().value("width").toInt(0);
    QString rawFps = streams.at(0).toObject().value("avg_frame_rate").toString();
    QString rawDate = streams.at(0).toObject().value("tags").toObject().value("creation_time").toString();


    VideoInfo videoInfo;
    bool integerA, integerB;

    // Height and width
    videoInfo.resolution = QSize(width, height);

    // FPS
    QStringList operandsFps = rawFps.split("/");
    if (operandsFps.size() == 2) {
        int a = operandsFps.at(0).toInt(&integerA);
        int b = operandsFps.at(1).toInt(&integerB);
        videoInfo.frameRate = (float) a/ (float) b;
    }

    // Capture date
    QDateTime date = QDateTime::fromString(rawDate, Qt::ISODateWithMs);
    if (date.isValid()) {
        videoInfo.date = date;
    }

    // Length
    float ms = rawLength.trimmed().toDouble()*1000;
    QTime length = QTime::fromMSecsSinceStartOfDay(ms);
    videoInfo.length = length;

    qDebug() << "Video info for" << video->fileName() << "took" << tmr.elapsed() << "ms";

    return videoInfo;
}

QList<int> MediaInfo::convertToHMS(qint64 miliseconds)
{
    qint64 seconds = miliseconds / 1000;
    int hours = seconds / 3600;
    for (int i=0; i<hours; i++) seconds -= 3600;
    int minutes = seconds / 60;
    for (int i=0; i<minutes; i++) seconds -= 60;
    return {hours, minutes, (int) seconds};
}


float MediaInfo::getFPS(QFile *video)
{
    return getVideoInfo(video).frameRate;
}

QSize MediaInfo::getResolution(QFile *video)
{
    return getVideoInfo(video).resolution;
}

QDateTime MediaInfo::getDate(QFile *media)
{
    return getVideoInfo(media).date;
}

QTime MediaInfo::getLength(QFile *media1)
{
    return getVideoInfo(media1).length;
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
    QMimeType type = db.mimeTypeForFile(media->fileName(), QMimeDatabase::MatchContent);
    return type;
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
