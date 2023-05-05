#include "fvideo.h"

FVideo::FVideo(QObject *parent, int id)
    : QObject{parent}
{
    this->id = id;
}

bool FVideo::addSegment(FSegment *segment)
{
    connect(segment, SIGNAL(verifyError(QString)), this, SLOT(segmentVerifyError(QString)));
    if (!segment->verify()) return false;
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

void FVideo::segmentVerifyError(QString error)
{
    emit verifyError("A segment is invalid. " + error);
}
