#ifndef FFORMATS_H
#define FFORMATS_H

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMediaPlayer>
#include <QMediaMetaData>
#include <QFileInfo>
#include <QDir>
#include <QSize>

enum FusionFormats {
    FUSION_VIDEO,
    FUSION_LOW_VIDEO,
    FUSION_AUDIO,
    FUSION_THUMNAIL
};

#include "fformat.h"

class FFormats : public QObject
{
    Q_OBJECT
public:
    static FFormat* get(QFile *media, int type);
    static FFormat getByName(QString name);

signals:

private:
    static QList<FFormat> formats;
    static const int thumbHeight = 120;
    static const int thumbWidth = 160;
    static const int lowVideoHeight = 720;
    static const int lowVideoWidth = 736;
    static const int lowVideoFps = 25;

};

#endif // FFORMATS_H
