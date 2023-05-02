#include "fsegment.h"

FSegment::FSegment(QObject *parent, int video_id, int id)
    : QObject{parent}
{
    this->id = id;
    this->video_id = video_id;
}

FSegment::FSegment(QObject *parent, int video_id, int id, QFile *front_mp4, QFile *front_lrv, QFile *front_thm, QFile *back_mp4, QFile *back_lrv, QFile *back_thm, QFile *back_wav)
{
    this->setParent(parent);
    this->video_id = video_id;
    this->id = id;
    this->front_mp4 = front_mp4;
    this->front_lrv = front_lrv;
    this->front_thm = front_thm;
    this->back_mp4 = back_mp4;
    this->back_lrv = back_lrv;
    this->back_thm = back_thm;
    this->back_wav = back_wav;
}

bool FSegment::verify()
{
    return true;
}

QString FSegment::toString()
{
    return  "ID: " + QString::number(id) +
            "VIDEO ID: " + QString::number(video_id) +
            "FRONT MP4: " + front_mp4->fileName() +
            "FRONT LRV: " + front_lrv->fileName() +
            "FRONT THM: " + front_thm->fileName() +
            "BACK MP4: " + back_mp4->fileName() +
            "BACK LRV: " + back_lrv->fileName() +
            "BACK THM: " + back_thm->fileName() +
            "BACK WAV: " + back_wav->fileName();

}
