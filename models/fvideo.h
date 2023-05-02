#ifndef QVIDEO_H
#define QVIDEO_H

#include <QObject>

#include "fsegment.h"

class FVideo : public QObject
{
    Q_OBJECT
public:
    explicit FVideo(QObject *parent = nullptr, int id = -1);
    bool addSegment(FSegment *segment);
    QString verify();

    QString toString();

signals:

private:
    int id;

    QList<FSegment*> segments;

};

#endif // QVIDEO_H
