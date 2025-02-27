#include "fvideo.h"
#include "utils/mediainfo.h"

FVideo::FVideo(QObject* parent, int id)
    : QObject{parent}
{
    this->id = id;
}

FVideo::~FVideo()
{
    qDeleteAll(segments);
    segments.clear();
    delete frontThumbnail;
    delete backThumbnail;
    delete dualFisheye;
    delete dualFisheyeLow;
    delete equirectangular;
    delete equirectangularLow;
}

void FVideo::moveToNewThread(QThread *newThread)
{
    this->moveToThread(newThread);

    for (FSegment* segment : segments) {
        if (segment) {
            segment->moveToThread(newThread);
        }
    }
}

bool FVideo::addSegment(FSegment* segment, VerifyMode verifyMode)
{
    if (segment == nullptr) return false;
    if (!segment->verify(verifyMode)) return false;
    this->segments.append(segment);
    return true;
}

bool FVideo::verify()
{
    for (FSegment *segment: this->segments) {
        qDebug() << "Verifying segment " << getIdString() + segment->getIdString();
        if (!segment->verify()) return false;
    }
    return true;
}

int FVideo::getId()
{
    return this->id;
}

FSegment* FVideo::getSegment(int i)
{
    if (i >= segments.length()) {
        qWarning() << "Requested segment " + QString::number(i) + " of video " + getIdString() +
                    " failed. Out of bounds for length " + QString::number(segments.length());
        return nullptr;
    }
    return segments.at(i);
}

QList<FSegment*> FVideo::getSegments()
{
    return segments;
}

int FVideo::getNumSegments()
{
    return this->segments.length();
}

QDateTime FVideo::getDate()
{
    if (!date.isValid()) {
        if (!segments.empty()) {
            date = MediaInfo::getDate(segments.at(0)->getFrontMP4());
        }
    }
    return date;
}

QFile* FVideo::getFrontThumbnail()
{
    if (frontThumbnail == nullptr) {
        qWarning() << "Could not get the front thumnail from video " << id;
        return new QFile(DEFAULT_THUMBNAIL_PATH);
    }
    return frontThumbnail;
}

QFile* FVideo::getBackThumbnail()
{
    if (backThumbnail == nullptr) {
        qWarning() << "Could not get the back thumnail from video " << id;
        return new QFile(DEFAULT_THUMBNAIL_PATH);
    }
    return backThumbnail;
}

QString FVideo::getIdString()
{
    QString vidIdString = QString::number(id);
    while (vidIdString.length() < 4) vidIdString.insert(0, "0");
    return vidIdString;
}

FFormat FVideo::getFormat()
{
    return (segments.length() > 0) ?  segments.at(0)->getFormat() : FFormat();
}

QTime FVideo::getLength()
{
    QTime totalLength(0,0);
    long totalMs = 0.0;
    for (FSegment *segment: segments) {
        totalMs += totalLength.msecsTo(segment->getLength());
    }
    return totalLength.addMSecs(totalMs);
}

bool FVideo::setFrontThumbnail(QString thumbnailPath)
{
    QFile* proposedThumbnail = new QFile(thumbnailPath);

    if (!proposedThumbnail->exists()) {
        qWarning() << "Tried to set a front thumbnail for video"
                   << getIdString() << "which path does not exist in"
                   << thumbnailPath;
        return false;
    }

    if (!MediaInfo::isImage(proposedThumbnail)) {
        qWarning() << "Tried to set a front thumbnail for video"
                   << getIdString() << "that is not an image";
        return false;
    }

    this->frontThumbnail = proposedThumbnail;
    return true;
}

bool FVideo::setBackThumbnail(QString thumbnailPath)
{
    QFile* proposedThumbnail = new QFile(thumbnailPath);

    if (!proposedThumbnail->exists()) {
        qWarning() << "Tried to set a back thumbnail for video"
                   << getIdString() << "which path does not exist in"
                   << thumbnailPath;
        return false;
    }

    if (!MediaInfo::isImage(proposedThumbnail)) {
        qWarning() << "Tried to set a back thumbnail for video"
                   << getIdString() << "that is not an image";
        return false;
    }

    this->backThumbnail = proposedThumbnail;
    return true;
}

void FVideo::setDualFisheye(QFile *dualFisheye)
{
    this->dualFisheye = dualFisheye;
}

QFile *FVideo::getDualFisheye()
{
    return dualFisheye;
}

void FVideo::setDualFisheyeLow(QFile *dualFisheyeLow)
{
    this->dualFisheyeLow = dualFisheyeLow;
}

QFile *FVideo::getDualFisheyeLow()
{
    return dualFisheyeLow;
}

bool FVideo::isDualFisheyeValid()
{
    if (dualFisheye == nullptr) return false;
    QTime a = MediaInfo::getLength(dualFisheye);
    QTime b = getLength();
    if (a.minute() == b.minute()
        && a.second() == b.second()) {
        return true;
    }
    return false;
}

bool FVideo::isDualFisheyeLowValid()
{
    if (dualFisheyeLow == nullptr) return false;
    QTime a = MediaInfo::getLength(dualFisheyeLow);
    QTime b = getLength();
    if (a.minute() == b.minute()
        && a.second() == b.second()) {
        return true;
    }
    return false;
}

bool FVideo::isEquirectangularValid()
{
    if (equirectangular == nullptr) return false;
    QTime a = MediaInfo::getLength(equirectangular);
    QTime b = getLength();
    if (a.minute() == b.minute()
        && a.second() == b.second()) {
        return true;
    }
    return false;
}

bool FVideo::isEquirectangularLowValid()
{
    if (equirectangularLow == nullptr) return false;
    QTime a = MediaInfo::getLength(equirectangularLow);
    QTime b = getLength();
    if (a.minute() == b.minute()
        && a.second() == b.second()) {
        return true;
    }
    return false;
}

bool FVideo::isFrontThumbnailValid()
{
    return (frontThumbnail == nullptr);
}

bool FVideo::isBackThumbnailValid()
{
    return (backThumbnail == nullptr);
}

QFile *FVideo::getEquirectangular()
{
    return equirectangular;
}

void FVideo::setEquirectangular(QFile *equirectangular)
{
    this->equirectangular = equirectangular;
}

QFile* FVideo::getEquirectangularLow()
{
    return equirectangularLow;
}

void FVideo::setEquirectangularLow(QFile *equirectangularLow)
{
    this->equirectangularLow = equirectangularLow;
}

QString FVideo::toString()
{
    QString ret;
    for (FSegment *segment: this->segments) {
        ret.append("NUEVO SEGMENTO\n");
        ret.append(segment->toString() + "\n");
        ret.append("FIN SEGMENTO\n");
    }
    return ret;
}

QJsonObject FVideo::toJson()
{
    QJsonObject videoObject;
    videoObject.insert("id", id);
    videoObject.insert("dualFisheye", isDualFisheyeValid());
    videoObject.insert("dualFisheyeLow", isDualFisheyeLowValid());
    videoObject.insert("equirectangular", isEquirectangularValid());
    videoObject.insert("equirectangularLow", isEquirectangularLowValid());
    videoObject.insert("frontThumbnail", isFrontThumbnailValid());
    videoObject.insert("backThumbnail", isBackThumbnailValid());
    QJsonArray segmentsArray;
    for (FSegment* segment: segments) {
        segmentsArray.append(segment->toJson());
    }
    videoObject.insert("segments", segmentsArray);
    return videoObject;
}
