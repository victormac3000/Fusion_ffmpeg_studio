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

#include "fformat.h"

class FFormats : public QObject
{
    Q_OBJECT
public:
    explicit FFormats(QObject *parent = nullptr);
    FFormat* get(QFile *video);

signals:

private:
    QList<FFormat> formats;

};

#endif // FFORMATS_H
