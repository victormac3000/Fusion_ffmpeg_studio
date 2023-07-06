#ifndef QVIDEO_H
#define QVIDEO_H

#include <QObject>

#include "models/fsegment.h"

struct FFmpegStatus {
    int frame;
    int fps;
    int quality;
    qint64 size;
    QTime elapsedTime;
    int bitrate;
    float speed;
    float percent;
};

class FVideo : public QObject
{
    Q_OBJECT
public:
    explicit FVideo(QObject *parent = nullptr, int id = -1);
    bool addSegment(FSegment *segment);
    bool verify();
    int getId();
    FSegment* getSegment(int i);
    int getNumSegments();
    QString getIdString();

    FFormat getFormat();
    QTime getLength();

    QString toString();

signals:
    void verifyError(QString error);

private slots:
    void segmentVerifyError(QString error);

private:
    int id;
    QList<FSegment*> segments;
    bool loadingPreview;



};

struct RenderItem {
    FVideo *video;
    int type;
    QTime start;
    QTime end;
};

#endif // QVIDEO_H
