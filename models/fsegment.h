#ifndef QSEGMENT_H
#define QSEGMENT_H

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QTime>
#include <QSize>

#include "models/fformat.h"

enum VerifyMode {
    FILES_ONLY, FULL
};

class FSegment : public QObject
{
    Q_OBJECT
public:
    explicit FSegment(QObject *parent = nullptr, int id = -1, QFile *front_mp4 = nullptr, QFile *front_lrv = nullptr,
                      QFile *back_mp4 = nullptr, QFile *back_lrv = nullptr, QFile *back_wav = nullptr);
    ~FSegment();


    int getId();
    QString getIdString();


    QString toString();

    bool verify(VerifyMode verifyMode = FULL);

    QFile *getFrontMP4();
    QFile *getFrontLRV();
    QFile *getBackMP4();
    QFile *getBackLRV();
    QFile *getBackWAV();

    void setDualFisheye(QFile *dualFisheye);
    QFile *getDualFisheye();


    void setDualFisheyeLow(QFile *dualFisheyeLow);
    QFile *getDualFisheyeLow();

    bool isDualFisheyeValid();
    bool isDualFisheyeLowValid();

    FFormat getFormat();
    float getFPS();
    QSize getResolution();
    QTime getLength();

    QJsonObject toJson();

signals:

private:
    int id;

    QFile *frontMP4 = nullptr;
    QFile *frontLRV = nullptr;

    QFile *backMP4 = nullptr;
    QFile *backLRV = nullptr;
    QFile *backWAV = nullptr;

    QFile *dualFisheye = nullptr;
    QFile *dualFisheyeLow = nullptr;

    QFile *equirectangular = nullptr;
    QFile *equirectangularLow = nullptr;

    FFormat format{};
    float fps;
    QSize resolution;
    QTime length;


};

#endif // QSEGMENT_H
