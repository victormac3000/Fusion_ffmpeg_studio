#ifndef FFMPEG_H
#define FFMPEG_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QProcess>
#include <QSettings>
#include <QRegularExpression>

#include "models/fvideo.h"
#include "models/renderwork.h"

const int TO_DUAL_FISHEYE = 0;

class FFmpeg : public QObject
{
    Q_OBJECT
public:
    explicit FFmpeg(QObject *parent = nullptr);
    ~FFmpeg();

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
    QProcess *process;
    QSettings settings;
    RenderWork *work;

    QString getPath();



};

#endif // FFMPEG_H
