#ifndef FFORMAT_H
#define FFORMAT_H

#include <QString>

struct video {
    int height, width, fps;
};

struct lowVideo {
    int height, width, fps;
};

struct thumnail {
    int height, width;
};

struct FFormat {
    QString name;
    float firmwareVersion;
    video video;
    lowVideo lowVideo;
    thumnail thumbnail;
};

#endif // FFORMAT_H
