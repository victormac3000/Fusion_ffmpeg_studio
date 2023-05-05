#ifndef QVIDEO_H
#define QVIDEO_H

#include <QObject>

#include "models/fsegment.h"

class FVideo : public QObject
{
    Q_OBJECT
public:
    explicit FVideo(QObject *parent = nullptr, int id = -1);
    bool addSegment(FSegment *segment);
    bool verify();
    int getId();
    FSegment* getSegment(int i);
    QString getIdString();

    QString toString();

signals:
    void verifyError(QString error);

private slots:
    void segmentVerifyError(QString error);

private:
    int id;
    QList<FSegment*> segments;



};

#endif // QVIDEO_H
