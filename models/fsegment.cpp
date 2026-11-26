#include "fsegment.h"
#include "utils/fformats.h"
#include "utils/toolbox.h"

FSegment::FSegment(int vid, int id)
{
    this->vid = vid;
    this->id = id;
}

FSegment FSegment::fromRow(int vid, const QVariant &row, QString projectDirPath)
{
    QVariantMap map = row.toMap();

    FSegment instance(vid, map.value("id").toInt());

    QStringList mediaPaths = {
        map.value("frontMP4").toString(),
        map.value("frontLRV").toString(),
        map.value("backMP4").toString(),
        map.value("backLRV").toString(),
        map.value("backWAV").toString()
    };

    if (!projectDirPath.isEmpty()) {
        for (int i=0; i<mediaPaths.length(); i++) {
            mediaPaths[i] = Toolbox::toAbsolutePath(mediaPaths[i], projectDirPath);
        }
    }

    instance.setFrontFiles(mediaPaths[0], mediaPaths[1]);
    instance.setBackFiles(mediaPaths[2], mediaPaths[3], mediaPaths[4]);

    return instance;
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

void FSegment::generateVideosInfo()
{
    if (!this->frontMP4.isEmpty()) {
        this->front_video_info = MediaInfo::getVideoInfo(this->frontMP4);
    }
    if (!this->backMP4.isEmpty()) {
        this->back_video_info = MediaInfo::getVideoInfo(this->frontMP4);
    }
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

    this->generateVideosInfo();

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

    this->generateVideosInfo();

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

VideoInfo FSegment::getFrontVideoInfo()
{
    return this->front_video_info;
}

VideoInfo FSegment::getBackVideoInfo()
{
    return this->back_video_info;
}

QVariantMap FSegment::toVariantMap(QString projectDirPath)
{
    QVariantMap map;

    QStringList mediaPaths = {
        frontMP4, frontLRV,
        backMP4, backLRV, backWAV
    };

    if (!projectDirPath.isEmpty()) {
        for (int i=0; i<mediaPaths.length(); i++) {
            mediaPaths[i] = Toolbox::toRelativePath(mediaPaths[i], projectDirPath);
        }
    }

    map.insert("id", id);
    map.insert("vid", vid);

    map.insert("frontMP4", mediaPaths[0]);
    map.insert("frontLRV", mediaPaths[1]);

    map.insert("backMP4", mediaPaths[2]);
    map.insert("backLRV", mediaPaths[3]);
    map.insert("backWAV", mediaPaths[4]);

    return map;
}