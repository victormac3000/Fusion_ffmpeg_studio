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

    bool isRunning();

signals:
    void renderDone(RenderWork *work, bool error);

public slots:
    void render(RenderWork *work);

private slots:
    void processStarted();
    void processStateChanged(QProcess::ProcessState newState);
    void processReadyReadOut();
    void processReadyReadError();
    void processErrorOccurred(QProcess::ProcessError error);
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

    void renderDone();

private:
    QProcess *process;
    QList<QString> params;
    QSettings settings;
    bool running = false;
    int exitCode = 1000;
    RenderWork *work;

    QString getPath();



};

#endif // FFMPEG_H
