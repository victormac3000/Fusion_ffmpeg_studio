#ifndef QVIDEO_H
#define QVIDEO_H

#include <QObject>

#include "utils/fformats.h"
#include "models/fsegment.h"

#define DEFAULT_THUMBNAIL_PATH "qrc:/Qml/Icons/VideoPlayer/no_video.png"

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

class FVideo
{
public:
    FVideo(int id = -1);
    int getId();
    QString getIdString();

    bool setFrontThumbnail(QString thumbnailPath);
    bool setBackThumbnail(QString thumbnailPath);
    void setFormat(FFormat format);
    bool addSegment(FSegment segment);

    QString getFrontThumbnail();
    QString getBackThumbnail();
    FFormat getFormat();
    QList<FSegment> getSegments();

private:
    int id;

    QString frontThumbnail;
    QString backThumbnail;
    FFormat format;

    QList<FSegment> segments;
};

struct RenderItem {
    FVideo *video;
    int type;
    QTime start;
    QTime end;
};

#endif // QVIDEO_H
