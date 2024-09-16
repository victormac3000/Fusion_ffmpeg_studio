#ifndef QSEGMENT_H
#define QSEGMENT_H

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QTime>
#include <QSize>

#include "models/fformat.h"

class FSegment : public QObject
{
    Q_OBJECT
public:
    explicit FSegment(QObject *parent = nullptr, int id = -1, QFile *front_mp4 = nullptr, QFile *front_lrv = nullptr, QFile *front_thm = nullptr,
                      QFile *back_mp4 = nullptr, QFile *back_lrv = nullptr, QFile *back_thm = nullptr, QFile *back_wav = nullptr);
    ~FSegment();


    int getId();
    QString getIdString();


    QString toString();

    bool verify();

    QFile *getFrontMP4() const;
    QFile *getFrontLRV() const;
    QFile *getFrontTHM() const;
    QFile *getBackMP4() const;
    QFile *getBackLRV() const;
    QFile *getBackTHM() const;
    QFile *getBackWAV() const;

    void setDualFisheye(QFile *dualFisheye);
    QFile *getDualFisheye();
    bool isDualFisheyeValid();

    void setDualFisheyeLow(QFile *dualFisheyeLow);
    QFile *getDualFisheyeLow();
    bool isDualFisheyeLowValid();

    FFormat getFormat();
    float getFPS();
    QSize getResolution();
    QTime getLength();

signals:
    void verifyError(QString error);

private:
    int id;

    QFile *frontMP4 = nullptr;
    QFile *frontLRV = nullptr;
    QFile *frontTHM = nullptr;

    QFile *backMP4 = nullptr;
    QFile *backLRV = nullptr;
    QFile *backTHM = nullptr;
    QFile *backWAV = nullptr;

    QFile *dualFisheye = nullptr;
    QFile *dualFisheyeLow = nullptr;

    QFile *equirectangular = nullptr;
    QFile *equirectangularLow = nullptr;

    FFormat format;
    float fps;
    QSize resolution;
    QTime length;


};

#endif // QSEGMENT_H
