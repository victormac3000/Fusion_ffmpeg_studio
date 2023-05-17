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

FSegment* FVideo::getSegment(int i)
{
    if (i >= segments.length()) {
        qWarning() << "Requested segment " + QString::number(i) + " of video " + getIdString() +
                    " failed. Out of bounds for length " + QString::number(segments.length());
        return nullptr;
    }
    return segments.at(i);
}

int FVideo::getNumSegments()
{
    return this->segments.length();
}

QString FVideo::getIdString()
{
    QString vidIdString = QString::number(id);
    while (vidIdString.length() < 4) vidIdString.insert(0, "0");
    return vidIdString;
}

FFormat FVideo::getFormat()
{
    return (segments.length() > 0) ?  segments.at(0)->getFormat() : FFormat();
}

QTime FVideo::getLength()
{
    QTime totalLength(0,0);
    long totalMs = 0.0;
    for (FSegment *segment: segments) {
        totalMs += totalLength.msecsTo(segment->getLength());
    }
    return totalLength.addMSecs(totalMs);
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
