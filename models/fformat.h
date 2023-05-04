#ifndef FFORMAT_H
#define FFORMAT_H

#include <QString>

struct FFormat {
    QString name;
    float firmwareVersion;
    int height, width, fps;
};

#endif // FFORMAT_H
