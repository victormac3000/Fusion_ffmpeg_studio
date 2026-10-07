#ifndef MEDIAINFO_H
#define MEDIAINFO_H

#include <QSettings>
#include <QMediaPlayer>
#include <QMediaMetaData>
#include <QDir>
#include <QFile>
#include <QSize>
#include <QMimeDatabase>
#include <QMimeType>
#include <QImage>
#include <QDateTime>
#include <QTime>
#include <QThread>
#include <QTimer>
#include <QEventLoop>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

struct VideoInfo {
    bool isVideo = false;
    QSize resolution;
    double frameRate;
    QDateTime date;
    double length_s;
    QString codecName;
    QString codecLongName;
};

struct AudioInfo {
    bool isAudio = false;
};

struct ImageInfo {
    bool isImage = false;
    QSize size;
};

class MediaInfo
{
public:
    static VideoInfo getVideoInfo(QString path);
    static AudioInfo getAudioInfo(QString path);
    static ImageInfo getImageInfo(QString path);

private:
    static QMimeType getMimeType(QString path);
};

#endif // MEDIAINFO_H
