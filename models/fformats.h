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

const int FUSION_VIDEO = 0;
const int FUSION_LOW_VIDEO = 1;
const int FUSION_AUDIO = 2;
const int FUSION_THUMNAIL = 3;

#include "fformat.h"
#include "utils/mediainfo.h"

class FFormats : public QObject
{
    Q_OBJECT
public:
    explicit FFormats(QObject *parent = nullptr);

    FFormat* get(QFile *media, int type);

signals:

private:
    QList<FFormat> formats;

};

#endif // FFORMATS_H
