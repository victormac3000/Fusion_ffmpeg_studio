#ifndef QSEGMENT_H
#define QSEGMENT_H

#include <QObject>
#include <QFile>
#include <QFileInfo>

#include "models/fformats.h"

class FSegment : public QObject
{
    Q_OBJECT
public:
    explicit FSegment(QObject *parent = nullptr, int id = -1, QFile *front_mp4 = nullptr, QFile *front_lrv = nullptr, QFile *front_thm = nullptr,
                      QFile *back_mp4 = nullptr, QFile *back_lrv = nullptr, QFile *back_thm = nullptr, QFile *back_wav = nullptr);

    QString verify();

    QString getIdString();

    QString toString();

private:
    QObject *video;
    int id;

    QFile *front_mp4 = nullptr;
    QFile *front_lrv = nullptr;
    QFile *front_thm = nullptr;

    QFile *back_mp4 = nullptr;
    QFile *back_lrv = nullptr;
    QFile *back_thm = nullptr;
    QFile *back_wav = nullptr;




signals:

};

#endif // QSEGMENT_H
