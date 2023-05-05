#include "fsegment.h"
#include "models/fvideo.h"
#include "models/fformats.h"

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

bool FSegment::verify()
{
    FVideo *video = (FVideo*) this->parent();
    FFormats formats;

    FFormat *frontMP4Format = formats.get(frontMP4, FUSION_VIDEO);
    FFormat *frontLRVFormat = formats.get(frontLRV, FUSION_LOW_VIDEO);
    FFormat *frontTHMFormat = formats.get(frontTHM, FUSION_THUMNAIL);
    FFormat *backMP4Format = formats.get(backMP4, FUSION_VIDEO);
    FFormat *backLRVFormat = formats.get(backLRV, FUSION_LOW_VIDEO);
    FFormat *backTHMFormat = formats.get(backTHM, FUSION_THUMNAIL);
    FFormat *backWAVFormat = formats.get(backWAV, FUSION_AUDIO);

    if (frontMP4Format == nullptr)  {
        qWarning() << "FrontMP4 file is invalid. Raw segment: " + toString();
        emit verifyError("The front segment MP4 number " + getIdString() + " of video " + video->getIdString() + " has an invalid format, check if your fusion files settings are supported");
        return false;
    }

    if (frontLRVFormat == nullptr)  {
        qWarning() << "FrontLRV file is invalid. Raw segment: " + toString();
        emit verifyError("The front segment LRV number " + getIdString() + " of video " + video->getIdString() + " has an invalid format, check if your fusion files settings are supported");
        return false;
    }

    if (frontTHMFormat == nullptr)  {
        qWarning() << "FrontTHM file is invalid. Raw segment: " + toString();
        emit verifyError("The front segment THM number " + getIdString() + " of video " + video->getIdString() + " has an invalid format, check if your fusion files settings are supported");
        return false;
    }

    if (backMP4Format == nullptr)  {
        qWarning() << "BackMP4 file is invalid. Raw segment: " + toString();
        emit verifyError("The back segment MP4 number " + getIdString() + " of video " + video->getIdString() + " has an invalid format, check if your fusion files settings are supported");
        return false;
    }

    if (backLRVFormat == nullptr)  {
        qWarning() << "BackLRV file is invalid. Raw segment: " + toString();
        emit verifyError("The back segment LRV number " + getIdString() + " of video " + video->getIdString() + " has an invalid format, check if your fusion files settings are supported");
        return false;
    }

    if (backTHMFormat == nullptr)  {
        qWarning() << "BackTHM file is invalid. Raw segment: " + toString();
        emit verifyError("The back segment THM number " + getIdString() + " of video " + video->getIdString() + " has an invalid format, check if your fusion files settings are supported");
        return false;
    }

    if (backWAVFormat == nullptr)  {
        qWarning() << "BackWAV file is invalid. Raw segment: " + toString();
        emit verifyError("The back segment WAV number " + getIdString() + " of video " + video->getIdString() + " has an invalid format, check if your fusion files settings are supported");
        return false;
    }

    if (!MediaInfo::isSameLength(frontMP4, backMP4)) {
        qWarning() << "FrontMP4 and BackMP4 are not the same length. Raw segment: " + toString();
        emit verifyError("The front and back MP4 files numbers " + getIdString() + " of video " + video->getIdString() + " does not have the same length");
        return false;
    }

    return true;
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
