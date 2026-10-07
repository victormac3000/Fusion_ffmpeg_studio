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

class FSegment
{
public:
    FSegment(int vid, int id);

    int getVideoId();
    int getId();
    QString getIdString();

    bool setFrontFiles(QString videoPath, QString lrvPath);
    bool setBackFiles(QString videoPath, QString lrvPath, QString audioPath);

    QString getFrontVideo();
    QString getFrontLVideo();
    FFormat getFrontVideoFormat();

    QString getBackVideo();
    QString getBackLVideo();
    QString getBackAudio();

private:
    int vid;
    int id;

    QString frontMP4;
    QString frontLRV;

    QString backMP4;
    QString backLRV;
    QString backWAV;


};

#endif // QSEGMENT_H
