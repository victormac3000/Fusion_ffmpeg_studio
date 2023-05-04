#include "fsegment.h"
#include "models/fvideo.h"

FSegment::FSegment(QObject *parent, int id, QFile *front_mp4, QFile *front_lrv, QFile *front_thm, QFile *back_mp4, QFile *back_lrv, QFile *back_thm, QFile *back_wav)
    : QObject{parent}
{
    this->id = id;
    this->video = parent;
    this->front_mp4 = front_mp4;
    this->front_lrv = front_lrv;
    this->front_thm = front_thm;
    this->back_mp4 = back_mp4;
    this->back_lrv = back_lrv;
    this->back_thm = back_thm;
    this->back_wav = back_wav;
}

QString FSegment::verify()
{
    FFormats formats;
    if (formats.get(front_mp4) == nullptr) qWarning("The front is nullable");
    formats.get(front_lrv);
    formats.get(front_thm);
    formats.get(back_mp4);
    formats.get(back_lrv);
    formats.get(back_thm);
    formats.get(back_wav);
    return "";
}

QString FSegment::getIdString()
{
    QString idString = QString::number(id);
    while (idString.length() < 2) idString.insert(0, "0");
    return idString;
}

QString FSegment::toString()
{
    FVideo* video = (FVideo*) video;
    return  "ID: " + QString::number(id) +
           "VIDEO ID: " + QString::number(video->getId())  +
            "FRONT MP4: " + front_mp4->fileName() +
            "FRONT LRV: " + front_lrv->fileName() +
            "FRONT THM: " + front_thm->fileName() +
            "BACK MP4: " + back_mp4->fileName() +
            "BACK LRV: " + back_lrv->fileName() +
            "BACK THM: " + back_thm->fileName() +
            "BACK WAV: " + back_wav->fileName();

}
