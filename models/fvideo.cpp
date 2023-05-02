#include "fvideo.h"

FVideo::FVideo(QObject *parent, int id)
    : QObject{parent}
{
    this->id = id;
}

bool FVideo::addSegment(FSegment *segment)
{
    if (!segment->verify()) return false;
    this->segments.append(segment);
    return true;
}

QString FVideo::verify()
{
    for (FSegment *segment: this->segments) {
        if (!segment->verify()) return "Error";
    }
    return "";
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
