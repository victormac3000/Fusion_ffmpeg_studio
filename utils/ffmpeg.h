#ifndef FFMPEG_H
#define FFMPEG_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QProcess>
#include <QSettings>
#include <QRegularExpression>
#include <QProcess>
#include <QDir>
#include <QList>
#include <QPair>

extern "C" {
    #include <libavutil/avutil.h>
    #include <libavutil/version.h>
    #include <libavcodec/avcodec.h>
    #include <libavcodec/version.h>
    #include <libavformat/avformat.h>
    #include <libavformat/version.h>
    #include <libavfilter/avfilter.h>
    #include <libavfilter/version.h>
    #include <libswscale/swscale.h>
    #include <libswscale/version.h>
    #include <libswresample/swresample.h>
    #include <libswresample/version.h>
    #include <libavdevice/avdevice.h>
    #include <libavdevice/version.h>
}

class RenderWork;
const int TO_DUAL_FISHEYE = 0;

class FFmpeg : public QObject
{
    Q_OBJECT
public:
    explicit FFmpeg(QObject *parent = nullptr);
    ~FFmpeg();

    QMap<QString,QString> getVersions();

signals:
    void renderDone(RenderWork *work, bool error);

public slots:
    void render(RenderWork *work);

private slots:
    void processStateChanged(QProcess::ProcessState newState);
    void processReadyReadOut();
    void processReadyReadError();
    void processErrorOccurred(QProcess::ProcessError error);

private:
    QString encoder;
    QProcess *process;
    QSettings settings;
    RenderWork *work;

    QDir dfLowSegmentsFolder;
    QDir dfLowVideosFolder;
    QDir dfSegmentsFolder;
    QDir dfVideosFolder;
    QDir equirectangularVideosFolder;
    QDir equirectangularLowVideosFolder;

    void renderPreview();
    bool renderPreviewStep1();
    bool renderPreviewStep2();
    bool renderPreviewStep3();

    QString getVersionsDots(unsigned int version);



};

#endif // FFMPEG_H
