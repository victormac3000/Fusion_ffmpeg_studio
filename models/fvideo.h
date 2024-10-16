#ifndef QVIDEO_H
#define QVIDEO_H

#include <QObject>

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

class FVideo : public QObject
{
    Q_OBJECT
public:
    explicit FVideo(int id = -1);
    ~FVideo();

    bool addSegment(FSegment *segment, VerifyMode verifyMode = FULL);
    bool verify();
    int getId();
    QDateTime getDate();
    FSegment* getSegment(int i);
    QList<FSegment*> getSegments();
    int getNumSegments();
    QString getIdString();

    FFormat getFormat();
    QTime getLength();

    void setFrontThumbnail(QFile *thumnail);
    QFile *getFrontThumbnail();

    void setBackThumbnail(QFile *thumnail);
    QFile *getBackThumbnail();

    void setDualFisheye(QFile *dualFisheye);
    QFile *getDualFisheye();
    void setDualFisheyeLow(QFile *dualFisheyeLow);
    QFile *getDualFisheyeLow();
    QFile *getEquirectangular();
    void setEquirectangular(QFile *equirectangular);

    QFile *getEquirectangularLow();
    void setEquirectangularLow(QFile *equirectangularLow);

    qint64 isDualFisheyeValid();
    qint64 isDualFisheyeLowValid();
    qint64 isEquirectangularValid();
    qint64 isEquirectangularLowValid();
    qint64 isFrontThumbnailValid();
    qint64 isBackThumbnailValid();

    QString toString();
    QJsonObject toJson();

private:
    int id;
    QList<FSegment*> segments;
    bool loadingPreview;
    QFile *dualFisheye = nullptr;
    QFile *dualFisheyeLow = nullptr;
    QFile *equirectangular = nullptr;
    QFile *equirectangularLow = nullptr;
    QFile *frontThumbnail = nullptr;
    QFile *backThumbnail = nullptr;
    QDateTime date;


};

struct RenderItem {
    FVideo *video;
    int type;
    QTime start;
    QTime end;
};

#endif // QVIDEO_H
