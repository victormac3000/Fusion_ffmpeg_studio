#include "fvideo.h"

FVideo::FVideo(QObject *parent, int id)
    : QObject{parent}
{
    this->id = id;
}

void FVideo::addSegment(FSegment *segment)
{
    if (!segment->verify().isEmpty()) {
        return;
    }
    this->segments.append(segment);
    return;
}

QString FVideo::verify()
{
    for (FSegment *segment: this->segments) {
        if (!segment->verify().isEmpty()) {
            return "Error";
        }
    }
    return "";
}

int FVideo::getId()
{
    return this->id;
}

QString FVideo::getIdString()
{
    QString vidIdString = QString::number(id);
    while (vidIdString.length() < 4) vidIdString.insert(0, "0");
    return vidIdString;
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
