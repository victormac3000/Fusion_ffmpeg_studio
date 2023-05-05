#include "mediainfo.h"


bool MediaInfo::isVideo(QFile *media)
{
    return getMimeType(media).preferredSuffix() == "mp4";
}

float MediaInfo::getFPS(QFile *video)
{
    return getMetadata(video).value(QMediaMetaData::VideoFrameRate).toFloat();
}

QSize MediaInfo::getResolution(QFile *video)
{
    return getMetadata(video).value(QMediaMetaData::Resolution).toSize();
}

QDateTime MediaInfo::getDate(QFile *media)
{
    return getMetadata(media).value(QMediaMetaData::Date).toDateTime();
}

bool MediaInfo::isSameLength(QFile *media1, QFile *media2)
{
    long media1Len = getMetadata(media1).value(QMediaMetaData::Duration).toLongLong() / 1000;
    long media2Len = getMetadata(media2).value(QMediaMetaData::Duration).toLongLong() / 1000;

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
    player.setSource(QUrl(media->fileName()));
    return player.metaData();
}
