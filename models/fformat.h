#ifndef FFORMAT_H
#define FFORMAT_H

#include <QString>

struct FFormat {
    QString name;
    QString firmware;
    int height = 0;
    int width = 0;
    double fps = 0.0;
};

#endif // FFORMAT_H
