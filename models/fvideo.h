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
    explicit FVideo(int id = -1);
    ~FVideo();

    bool addSegment(FSegment *segment, bool fileVerify = true);
    bool verify();
    int getId();
    FSegment* getSegment(int i);
    QList<FSegment*> getSegments();
    int getNumSegments();
    QString getIdString();

    FFormat getFormat();
    QTime getLength();

    void setDualFisheye(QFile *dualFisheye);
    QFile *getDualFisheye();
    void setDualFisheyeLow(QFile *dualFisheyeLow);
    QFile *getDualFisheyeLow();
    bool isDualFisheyeValid();
    bool isDualFisheyeLowValid();
    QFile *getEquirectangular();
    void setEquirectangular(QFile *equirectangular);
    bool isEquirectangularValid();
    QFile *getEquirectangularLow();
    void setEquirectangularLow(QFile *equirectangularLow);
    bool isEquirectangularLowValid();

    QString toString();

signals:
    void verifyError(QString error);

private slots:
    void segmentVerifyError(QString error);

private:
    int id;
    QList<FSegment*> segments;
    bool loadingPreview;
    QFile *dualFisheye = nullptr;
    QFile *dualFisheyeLow = nullptr;
    QFile *equirectangular = nullptr;
    QFile *equirectangularLow = nullptr;



};

struct RenderItem {
    FVideo *video;
    int type;
    QTime start;
    QTime end;
};

#endif // QVIDEO_H
