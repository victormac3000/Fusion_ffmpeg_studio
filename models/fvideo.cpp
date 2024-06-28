#include "fvideo.h"

FVideo::FVideo(int id)
    : QObject{nullptr}
{
    this->id = id;
}

FVideo::~FVideo()
{
    qDeleteAll(segments);
    segments.clear();
    delete dualFisheye;
    delete dualFisheyeLow;
    delete equirectangular;
    delete equirectangularLow;
}

bool FVideo::addSegment(FSegment *segment, bool fileVerify)
{
    connect(segment, SIGNAL(verifyError(QString)), this, SLOT(segmentVerifyError(QString)));
    if (fileVerify && !segment->verify()) return false;
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

void FVideo::setFrontThumbnail(QFile *thumbnail)
{
    this->frontThumbnail = thumbnail;
}

void FVideo::setBackThumbnail(QFile *thumbnail)
{
    this->backThumbnail = thumbnail;
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
    return a.minute() == b.minute()
        && a.second() == b.second();
}

bool FVideo::isDualFisheyeLowValid()
{
    if (dualFisheyeLow == nullptr) return false;
    QTime a = MediaInfo::getLength(dualFisheyeLow);
    QTime b = getLength();
    return a.minute() == b.minute()
           && a.second() == b.second();
}

QFile *FVideo::getEquirectangular()
{
    return equirectangular;
}

void FVideo::setEquirectangular(QFile *equirectangular)
{
    this->equirectangular = equirectangular;
}

bool FVideo::isEquirectangularValid()
{
    if (equirectangular == nullptr) return false;
    QTime a = MediaInfo::getLength(equirectangular);
    QTime b = getLength();
    return a.minute() == b.minute()
           && a.second() == b.second();
}

QFile* FVideo::getEquirectangularLow()
{
    return equirectangularLow;
}

void FVideo::setEquirectangularLow(QFile *equirectangularLow)
{
    this->equirectangularLow = equirectangularLow;
}

bool FVideo::isEquirectangularLowValid()
{
    if (equirectangularLow == nullptr) return false;
    QTime a = MediaInfo::getLength(equirectangularLow);
    QTime b = getLength();
    return a.minute() == b.minute()
           && a.second() == b.second();
}

QString FVideo::toString()
{
    QString ret;
    for (FSegment *segment: this->segments) {
        ret.append("NUEVO SEGMENTO");
        ret.append(segment->toString());
        ret.append("FIN SEGMENTO");
    }
    return ret;
}

void FVideo::segmentVerifyError(QString error)
{
    emit verifyError("A segment is invalid. " + error);
}
