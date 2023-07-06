#include "renderwork.h"

RenderWork::RenderWork(QObject *parent, FVideo *video, int type)
    : QObject{parent}
{
    this->video = video;
    this->type = type;
}

FVideo *RenderWork::getVideo()
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
        return "Rendering preview media";
    }
    return "Unknown work (May be a bug)";
}


