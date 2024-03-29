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
    QSize resolution;
    float frameRate;
    QDateTime date;
    QTime length;
};

class MediaInfo
{
public:
    static bool isVideo(QFile *media);
    static float getFPS(QFile *video);
    static QSize getResolution(QFile *video);
    static QDateTime getDate(QFile *video);
    static QTime getLength(QFile *media1);
    static bool isSameLength(QFile *media1, QFile *media2);
    static QSize getImageResolution(QFile *image);
    static bool isAudio(QFile *media);
    static bool isImage(QFile *media);
    static QMimeType getMimeType(QFile *media);
    static VideoInfo getVideoInfo(QFile *video);
    static QList<int> convertToHMS(qint64 miliseconds);

private:
    static QMediaMetaData getMetadata(QFile *media);
};

#endif // MEDIAINFO_H
