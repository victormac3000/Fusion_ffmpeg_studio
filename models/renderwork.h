#ifndef RENDERWORK_H
#define RENDERWORK_H

#include "models/fvideo.h"

#include <QObject>

#define GENERIC 0
#define RENDER_PREVIEW 1
#define RENDER_PREVIEW_360 2

class RenderWork : public QObject
{
    Q_OBJECT
public:
    explicit RenderWork(QObject *parent = nullptr, FVideo *video = nullptr, int type = GENERIC);
    FVideo* getVideo();
    int getType();
    QString getTypeString();

signals:
    void updateRenderStatus(FFmpegStatus *status);
    void updateRenderStatusString(QString statusString);

public slots:



private:
    FVideo *video;
    int type;



};

#endif // RENDERWORK_H
