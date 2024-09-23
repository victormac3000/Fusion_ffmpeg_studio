#include "fsegment.h"
#include "models/fvideo.h"
#include "models/fformats.h"
#include "utils/mediainfo.h"

FSegment::FSegment(QObject *parent, int id, QFile *front_mp4, QFile *front_lrv, QFile *front_thm, QFile *back_mp4, QFile *back_lrv, QFile *back_thm, QFile *back_wav)
    : QObject {parent}
{
    this->id = id;
    this->frontMP4 = front_mp4;
    this->frontLRV = front_lrv;
    this->frontTHM = front_thm;
    this->backMP4 = back_mp4;
    this->backLRV = back_lrv;
    this->backTHM = back_thm;
    this->backWAV = back_wav;
}

FSegment::~FSegment()
{

}

bool FSegment::verify()
{
    FVideo *video = (FVideo*) this->parent();
    if (video == nullptr) {
        qWarning() << "The segment has a nullptr video";
        return false;
    }

    FFormats formats;

    FFormat *frontMP4Format = formats.get(frontMP4, FUSION_VIDEO);
    FFormat *frontLRVFormat = formats.get(frontLRV, FUSION_LOW_VIDEO);
    FFormat *frontTHMFormat = formats.get(frontTHM, FUSION_THUMNAIL);
    FFormat *backMP4Format = formats.get(backMP4, FUSION_VIDEO);
    FFormat *backLRVFormat = formats.get(backLRV, FUSION_LOW_VIDEO);
    FFormat *backTHMFormat = formats.get(backTHM, FUSION_THUMNAIL);
    FFormat *backWAVFormat = formats.get(backWAV, FUSION_AUDIO);

    if (frontMP4Format == nullptr)  {
        qWarning() << "FrontMP4 file is invalid. Raw segment: " << toString();
        return false;
    }

    if (frontLRVFormat == nullptr)  {
        qWarning() << "FrontLRV file is invalid. Raw segment: " + toString();
        return false;
    }

    if (frontTHMFormat == nullptr)  {
        qWarning() << "FrontTHM file is invalid. Raw segment: " + toString();
        return false;
    }

    if (backMP4Format == nullptr)  {
        qWarning() << "BackMP4 file is invalid. Raw segment: " + toString();
        return false;
    }

    if (backLRVFormat == nullptr)  {
        qWarning() << "BackLRV file is invalid. Raw segment: " + toString();
        return false;
    }

    if (backTHMFormat == nullptr)  {
        qWarning() << "BackTHM file is invalid. Raw segment: " + toString();
        return false;
    }

    if (backWAVFormat == nullptr)  {
        qWarning() << "BackWAV file is invalid. Raw segment: " + toString();
        return false;
    }

    if (!MediaInfo::isSameLength(frontMP4, backMP4)) {
        qWarning() << "FrontMP4 and BackMP4 are not the same length. Raw segment: " + toString();
        return false;
    }

    this->format = *frontMP4Format;

    return true;
}

QFile *FSegment::getFrontMP4() const
{
    return frontMP4;
}

QFile *FSegment::getFrontLRV() const
{
    return frontLRV;
}

QFile *FSegment::getFrontTHM() const
{
    return frontTHM;
}

QFile *FSegment::getBackMP4() const
{
    return backMP4;
}

QFile *FSegment::getBackLRV() const
{
    return backLRV;
}

QFile *FSegment::getBackTHM() const
{
    return backTHM;
}

QFile *FSegment::getBackWAV() const
{
    return backWAV;
}

void FSegment::setDualFisheye(QFile *dualFisheye)
{
    this->dualFisheye = dualFisheye;
}

QFile *FSegment::getDualFisheye()
{
    return this->dualFisheye;
}

void FSegment::setDualFisheyeLow(QFile *dualFisheyeLow)
{
    this->dualFisheyeLow = dualFisheyeLow;
}

QFile *FSegment::getDualFisheyeLow()
{
    return this->dualFisheyeLow;
}

bool FSegment::isDualFisheyeValid()
{
    if (dualFisheye == nullptr) return false;
    QTime a = MediaInfo::getLength(dualFisheye);
    QTime b = getLength();
    return a.minute() == b.minute()
           && (abs(a.second()-b.second()) < 3);
}

bool FSegment::isDualFisheyeLowValid()
{
    if (dualFisheyeLow == nullptr) return false;
    QTime a = MediaInfo::getLength(dualFisheyeLow);
    QTime b = getLength();
    return a.minute() == b.minute()
           && (abs(a.second()-b.second()) < 3);
}

FFormat FSegment::getFormat()
{
    return format;
}

float FSegment::getFPS()
{
    if (fps <= 0) {
        fps = MediaInfo::getFPS(frontMP4);
    }
    return fps;
}

QSize FSegment::getResolution()
{
    if (!resolution.isValid()) {
        resolution = MediaInfo::getResolution(frontMP4);
    }
    return resolution;
}

QTime FSegment::getLength()
{
    if (!length.isValid()) {
        length = MediaInfo::getLength(frontMP4);
    }
    return length;
}

int FSegment::getId()
{
    return id;
}

QString FSegment::getIdString()
{
    QString idString = QString::number(id);
    while (idString.length() < 2) idString.insert(0, "0");
    return idString;
}

QString FSegment::toString()
{
    FVideo* video = (FVideo*) this->parent();
    return  "ID: " + QString::number(id) +
           "VIDEO ID: " + QString::number(video->getId())  +
           "FRONT MP4: " + frontMP4->fileName() +
           "FRONT LRV: " + frontLRV->fileName() +
           "FRONT THM: " + frontTHM->fileName() +
           "BACK MP4: " + backMP4->fileName() +
           "BACK LRV: " + backLRV->fileName() +
           "BACK THM: " + backTHM->fileName() +
           "BACK WAV: " + backWAV->fileName();

}
