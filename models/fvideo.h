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
    explicit FVideo(QObject* parent = nullptr, int id = -1);
    ~FVideo();

    void moveToNewThread(QThread* newThread);

    bool addSegment(FSegment* segment, VerifyMode verifyMode = FULL);
    bool verify();
    int getId();
    QDateTime getDate();
    FSegment* getSegment(int i);
    QList<FSegment*> getSegments();
    int getNumSegments();
    QString getIdString();

    FFormat getFormat();
    QTime getLength();

    bool setFrontThumbnail(QString thumbnailPath);
    QFile *getFrontThumbnail();

    bool setBackThumbnail(QString thumbnailPath);
    QFile *getBackThumbnail();

    void setDualFisheye(QFile *dualFisheye);
    QFile *getDualFisheye();
    void setDualFisheyeLow(QFile *dualFisheyeLow);
    QFile *getDualFisheyeLow();
    QFile *getEquirectangular();
    void setEquirectangular(QFile *equirectangular);

    QFile *getEquirectangularLow();
    void setEquirectangularLow(QFile *equirectangularLow);

    bool isDualFisheyeValid();
    bool isDualFisheyeLowValid();
    bool isEquirectangularValid();
    bool isEquirectangularLowValid();
    bool isFrontThumbnailValid();
    bool isBackThumbnailValid();

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
