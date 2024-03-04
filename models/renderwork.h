#ifndef RENDERWORK_H
#define RENDERWORK_H

#include "models/fvideo.h"
#include "models/project.h"

#include <QObject>

#define GENERIC 0
#define RENDER_PREVIEW 1
#define RENDER_PREVIEW_360 2

class RenderWork : public QObject
{
    Q_OBJECT
public:
    explicit RenderWork(QObject *parent = nullptr, Project *project = nullptr, FVideo *video = nullptr, int type = GENERIC);
    Project* getProject();
    FVideo* getVideo();
    int getType();
    QString getTypeString();
    void setOverwrite(bool overwrite);
    bool getOverwrite();

signals:
    void updateRenderStatus(FFmpegStatus* status);
    void updateRenderStatusString(QString statusString);

public slots:



private:
    Project *project;
    FVideo *video;
    int type;
    bool overwrite;



};

#endif // RENDERWORK_H
