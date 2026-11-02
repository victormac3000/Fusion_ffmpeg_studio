#include "fsegment.h"
#include "fvideo.h"
#include "utils/fformats.h"
#include "utils/mediainfo.h"

FVideo::FVideo(int id)
{
    this->id = id;
}

int FVideo::getId()
{
    return this->id;
}

QString FVideo::getIdString()
{
    QString vidIdString = QString::number(id);
    while (vidIdString.length() < 4) vidIdString.insert(0, "0");
    return vidIdString;
}

bool FVideo::setFrontThumbnail(QString thumbnailPath)
{
    FFormat format = FFormats::get(thumbnailPath, FUSION_THUMNAIL);
    if (format.name.isEmpty()) {
        qWarning() << "Tried to set an invalid front thumbnail for video"
                   << getIdString();
        return false;
    }

    this->frontThumbnail = thumbnailPath;
    return true;
}

bool FVideo::setBackThumbnail(QString thumbnailPath)
{
    FFormat format = FFormats::get(thumbnailPath, FUSION_THUMNAIL);
    if (format.name.isEmpty()) {
        qWarning() << "Tried to set an invalid back thumbnail for video"
                   << getIdString();
        return false;
    }

    this->backThumbnail = thumbnailPath;
    return true;
}

void FVideo::setFormat(FFormat format)
{
    this->format = format;
}

bool FVideo::addSegment(FSegment segment)
{
    if (this->id != segment.getVideoId()) {
        qDebug() << "Tried to add a segment that does not belong to this video";
        return false;
    }
    this->segments.append(segment);
    return true;
}

QString FVideo::getFrontThumbnail()
{
    return this->frontThumbnail;
}

QString FVideo::getBackThumbnail()
{
    return this->backThumbnail;
}

QList<FSegment> FVideo::getSegments()
{
    return segments;
}

FFormat FVideo::getFormat()
{
    return this->format;
}

QVariantMap FVideo::toVariantMap()
{
    QFile frontThumnailFile(this->frontThumbnail);
    QFile backThumnailFile(this->backThumbnail);

    QString frontThumbnailB64;
    QString backThumbnailB64;

    if (frontThumnailFile.open(QFile::ReadOnly)) {
        frontThumbnailB64 = frontThumnailFile.readAll().toBase64();
        frontThumnailFile.close();
    }

    if (backThumnailFile.open(QFile::ReadOnly)) {
        backThumbnailB64 = backThumnailFile.readAll().toBase64();
        backThumnailFile.close();
    }

    QDateTime recordedOn = MediaInfo::getVideoInfo(this->segments.first().getFrontVideo()).date;

    QVariantMap map;

    map.insert("id", this->id);
    map.insert("idString", this->getIdString());
    map.insert("frontThumbnail", this->frontThumbnail);
    map.insert("frontThumbnailB64", frontThumbnailB64);
    map.insert("backThumbnail", this->backThumbnail);
    map.insert("backThumbnailB64", backThumbnailB64);
    map.insert("front", this->segments.first().getFrontVideo());
    map.insert("frontURL", QUrl::fromLocalFile(this->segments.first().getFrontVideo()));
    map.insert("back", this->segments.first().getBackVideo());
    map.insert("backURL", QUrl::fromLocalFile(this->segments.first().getBackVideo()));
    map.insert("recordedOnDate", recordedOn.toString("dd/MM/yyyy"));

    return map;
}