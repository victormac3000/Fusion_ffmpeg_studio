#include "renderwork.h"

RenderWork::RenderWork(QObject *parent, Project *project, FVideo *video, int type)
    : QObject{parent}
{
    this->project = project;
    this->video = video;
    this->type = type;
}

Project* RenderWork::getProject()
{
    return this->project;
}

FVideo* RenderWork::getVideo()
{
    return video;
}

int RenderWork::getType()
{
    return type;
}

QString RenderWork::getTypeString()
{
    switch (type) {
    case RENDER_PREVIEW:
        return "Render preview media";
    }
    return "Unknown work (May be a bug)";
}

bool RenderWork::getOverwrite()
{
    return overwrite;
}

void RenderWork::setOverwrite(bool overwrite)
{
    this->overwrite = overwrite;
}


