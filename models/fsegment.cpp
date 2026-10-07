#include "fsegment.h"
#include "models/fvideo.h"
#include "utils/fformats.h"
#include "utils/mediainfo.h"

FSegment::FSegment(int vid, int id)
{
    this->vid = vid;
    this->id = id;
}

int FSegment::getId()
{
    return this->id;
}

int FSegment::getVideoId()
{
    return this->vid;
}

QString FSegment::getIdString()
{
    QString idString = QString::number(id);
    while (idString.length() < 2) idString.insert(0, "0");
    return idString;
}

bool FSegment::setFrontFiles(QString videoPath, QString lrvPath)
{   
    FFormat videoFormat = FFormats::get(videoPath, FUSION_VIDEO);
    FFormat lrvFormat = FFormats::get(lrvPath, FUSION_LOW_VIDEO);

    if (videoFormat.name.isEmpty() || lrvFormat.name.isEmpty()) {
        qDebug() << "video" << videoPath << "or lrv" << lrvPath << "are not in valid format";
        return false;
    }

    this->frontMP4 = videoPath;
    this->frontLRV = lrvPath;

    return true;
}

bool FSegment::setBackFiles(QString videoPath, QString lrvPath, QString audioPath)
{
    FFormat videoFormat = FFormats::get(videoPath, FUSION_VIDEO);
    FFormat lrvFormat = FFormats::get(lrvPath, FUSION_LOW_VIDEO);
    FFormat audioFormat = FFormats::get(audioPath, FUSION_AUDIO);

    if (videoFormat.name.isEmpty() || lrvFormat.name.isEmpty() || audioFormat.name.isEmpty()) {
        qDebug() << "video" << videoPath << "lrv" << lrvPath << "or audio" << audioPath << "are not in valid format";
        return false;
    }

    this->backMP4 = videoPath;
    this->backLRV = lrvPath;
    this->backWAV = audioPath;

    return true;
}

QString FSegment::getFrontVideo()
{
    return this->frontMP4;
}

QString FSegment::getFrontLVideo()
{
    return this->frontLRV;
}

FFormat FSegment::getFrontVideoFormat()
{
    return FFormats::get(this->frontMP4, FUSION_VIDEO);
}

QString FSegment::getBackVideo()
{
    return this->backMP4;
}

QString FSegment::getBackLVideo()
{
    return this->backLRV;
}

QString FSegment::getBackAudio()
{
    return this->backWAV;
}