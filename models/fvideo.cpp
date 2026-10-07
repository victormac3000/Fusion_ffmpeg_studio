#include "fsegment.h"
#include "fvideo.h"
#include "utils/fformats.h"

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