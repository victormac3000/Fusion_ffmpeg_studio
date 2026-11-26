#ifndef QSEGMENT_H
#define QSEGMENT_H

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QTime>
#include <QSize>

#include "models/fformat.h"
#include "utils/mediainfo.h"

class FSegment
{
public:
    FSegment(int vid, int id);
    static FSegment fromRow(int vid, const QVariant& row, QString projectDirPath = "");

    int getVideoId();
    int getId();
    QString getIdString();
    void generateVideosInfo();

    bool setFrontFiles(QString videoPath, QString lrvPath);
    bool setBackFiles(QString videoPath, QString lrvPath, QString audioPath);

    QString getFrontVideo();
    QString getFrontLVideo();
    FFormat getFrontVideoFormat();

    QString getBackVideo();
    QString getBackLVideo();
    QString getBackAudio();

    VideoInfo getFrontVideoInfo();
    VideoInfo getBackVideoInfo();

    QVariantMap toVariantMap(QString projectDirPath = "");

private:
    int id;
    int vid;

    QString frontMP4;
    QString frontLRV;

    QString backMP4;
    QString backLRV;
    QString backWAV;

    VideoInfo front_video_info;
    VideoInfo back_video_info;

};

#endif // QSEGMENT_H
