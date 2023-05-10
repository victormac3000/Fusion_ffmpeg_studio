#ifndef FFMPEG_H
#define FFMPEG_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QProcess>
#include <QSettings>

#include "models/fvideo.h"

class FFmpeg : public QObject
{
    Q_OBJECT
public:
    explicit FFmpeg(QObject *parent = nullptr);
    ~FFmpeg();

signals:
    void preRenderDone(FVideo *video);
    void preRenderError(FVideo *video, QString error);

public slots:
    void preRender(FVideo *video);

private slots:
    void processStarted();
    void processStateChanged(QProcess::ProcessState newState);
    void processReadyReadOut();
    void processReadyReadError();
    void processErrorOccurred(QProcess::ProcessError error);
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess *process;
    QList<QString> params;
    QSettings settings;

    QString getPath();



};

#endif // FFMPEG_H
