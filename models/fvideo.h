#ifndef QVIDEO_H
#define QVIDEO_H

#include <QObject>

#include "models/fsegment.h"

class FVideo : public QObject
{
    Q_OBJECT
public:
    explicit FVideo(QObject *parent = nullptr, int id = -1);
    void addSegment(FSegment *segment);
    QString verify();
    int getId();
    QString getIdString();

    QString toString();

signals:

private:
    int id;

    QList<FSegment*> segments;

};

#endif // QVIDEO_H
