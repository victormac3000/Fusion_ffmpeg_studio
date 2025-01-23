#include "fsegment.h"
#include "models/fvideo.h"
#include "models/fformats.h"
#include "utils/mediainfo.h"

FSegment::FSegment(QObject *parent, int id, QFile *front_mp4, QFile *front_lrv, QFile *back_mp4, QFile *back_lrv, QFile *back_wav)
    : QObject {parent}
{
    this->id = id;
    this->frontMP4 = front_mp4;
    this->frontLRV = front_lrv;
    this->backMP4 = back_mp4;
    this->backLRV = back_lrv;
    this->backWAV = back_wav;
}

FSegment::~FSegment()
{
    delete frontMP4;
    delete frontLRV;
    delete backMP4;
    delete backLRV;
    delete backWAV;
    delete dualFisheye;
    delete dualFisheyeLow;
    delete equirectangular;
    delete equirectangularLow;
}

bool FSegment::verify(VerifyMode verifyMode)
{
    FVideo *video = (FVideo*) this->parent();

    if (video == nullptr) {
        qWarning() << "The segment" << id << "has a nullptr video";
        return false;
    }

    if (frontMP4 == nullptr)  {
        qWarning() << "FrontMP4 file is null. Raw segment: \n" << toString();
        return false;
    }

    if (frontLRV == nullptr)  {
        qWarning() << "FrontLRV file is null. Raw segment: \n" + toString();
        return false;
    }

    if (backMP4 == nullptr)  {
        qWarning() << "BackMP4 file is null. Raw segment: \n" + toString();
        return false;
    }

    if (backLRV == nullptr)  {
        qWarning() << "BackLRV file is null. Raw segment: \n" + toString();
        return false;
    }

    if (backWAV == nullptr)  {
        qWarning() << "BackWAV file is null. Raw segment: \n" + toString();
        return false;
    }

    if (verifyMode == FULL) {
        FFormats formats;

        FFormat frontMP4Format = formats.get(frontMP4, FUSION_VIDEO);
        FFormat frontLRVFormat = formats.get(frontLRV, FUSION_LOW_VIDEO);
        FFormat frontTHMFormat = formats.get(video->getFrontThumbnail(), FUSION_THUMNAIL);
        FFormat backMP4Format = formats.get(backMP4, FUSION_VIDEO);
        FFormat backLRVFormat = formats.get(backLRV, FUSION_LOW_VIDEO);
        FFormat backTHMFormat = formats.get(video->getBackThumbnail(), FUSION_THUMNAIL);
        FFormat backWAVFormat = formats.get(backWAV, FUSION_AUDIO);
        
        if (frontMP4Format.name.isEmpty())  {
            qWarning() << "FrontMP4 file is invalid. Raw segment: \n" << toString();
            return false;
        }

        if (frontLRVFormat.name.isEmpty())  {
            qWarning() << "FrontLRV file is invalid. Raw segment: \n" + toString();
            return false;
        }

        if (frontTHMFormat.name.isEmpty())  {
            qWarning() << "FrontTHM file is invalid. Raw segment: \n" + toString();
            return false;
        }

        if (backMP4Format.name.isEmpty())  {
            qWarning() << "BackMP4 file is invalid. Raw segment: \n" + toString();
            return false;
        }

        if (backLRVFormat.name.isEmpty())  {
            qWarning() << "BackLRV file is invalid. Raw segment: \n" + toString();
            return false;
        }

        if (backTHMFormat.name.isEmpty())  {
            qWarning() << "BackTHM file is invalid. Raw segment: \n" + toString();
            return false;
        }

        if (backWAVFormat.name.isEmpty())  {
            qWarning() << "BackWAV file is invalid. Raw segment: \n" + toString();
            return false;
        }

        if (!MediaInfo::isSameLength(frontMP4, backMP4)) {
            qWarning() << "FrontMP4 and BackMP4 are not the same length. Raw segment: \n" + toString();
            return false;
        }

        this->format = frontMP4Format;

        return true;
    }

    if (format.name.isEmpty()) {
        qWarning() << "Format name is empty. Raw segment: " + toString();
        return false;
    }

    return true;
}

QFile *FSegment::getFrontMP4()
{
    return frontMP4;
}

QFile *FSegment::getFrontLRV()
{
    return frontLRV;
}

QFile *FSegment::getBackMP4()
{
    return backMP4;
}

QFile *FSegment::getBackLRV()
{
    return backLRV;
}

QFile *FSegment::getBackWAV()
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
    if (a.minute() == b.minute()
        && (abs(a.second()-b.second()) < 3)) {
        return true;
    }
    return false;
}

bool FSegment::isDualFisheyeLowValid()
{
    if (dualFisheyeLow == nullptr) return false;
    QTime a = MediaInfo::getLength(dualFisheyeLow);
    QTime b = getLength();
    if (a.minute() == b.minute()
        && (abs(a.second()-b.second()) < 3)) {
        return true;
    }
    return false;
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

QJsonObject FSegment::toJson()
{
    qint64 frontMP4Size = (frontMP4 != nullptr) ? frontMP4->size() : -1;
    qint64 frontLRVSize = (frontLRV != nullptr) ? frontLRV->size() : -1;
    qint64 backMP4Size = (backMP4 != nullptr) ? backMP4->size() : -1;
    qint64 backLRVSize = (backLRV != nullptr) ? backLRV->size() : -1;
    qint64 backWAVSize = (backWAV != nullptr) ? backWAV->size() : -1;

    QJsonObject segmentObject;
    segmentObject.insert("id", id);
    segmentObject.insert("dualFisheye", isDualFisheyeValid());
    segmentObject.insert("dualFisheyeLow", isDualFisheyeLowValid());
    segmentObject.insert("frontMP4", frontMP4Size);
    segmentObject.insert("frontLRV", frontLRVSize);
    segmentObject.insert("backMP4", backMP4Size);
    segmentObject.insert("backLRV", backLRVSize);
    segmentObject.insert("backWAV", backWAVSize);
    segmentObject.insert("format", format.name);
    return segmentObject;
}

QString FSegment::toString()
{
    FVideo* video = (FVideo*) this->parent();
    QString videoId = (video != nullptr) ? QString::number(video->getId()) : "NULL";
    QString frontMP4Path = (frontMP4 != nullptr) ? frontMP4->fileName() : "NULL";
    QString frontLRVPath = (frontLRV != nullptr) ? frontLRV->fileName() : "NULL";
    QString backMP4Path = (backMP4 != nullptr) ? backMP4->fileName() : "NULL";
    QString backLRVPath = (backLRV != nullptr) ? backLRV->fileName() : "NULL";
    QString backWAVPath = (backWAV != nullptr) ? backWAV->fileName() : "NULL";

    return  "ID: " + QString::number(id) +
            "VIDEO ID: " + videoId  + "\n" +
            "FRONT MP4: " + frontMP4Path + "\n" +
            "FRONT LRV: " + frontLRVPath + "\n" +
            "BACK MP4: " + backMP4Path + "\n" +
            "BACK LRV: " + backLRVPath + "\n" +
            "BACK WAV: " + backWAVPath + "\n" +
            "FORMAT NAME: " + format.name;

}
