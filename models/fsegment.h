#ifndef QSEGMENT_H
#define QSEGMENT_H

#include <QObject>
#include <QFile>
#include <QFileInfo>

class FSegment : public QObject
{
    Q_OBJECT
public:
    explicit FSegment(QObject *parent = nullptr, int video_id = -1, int id = -1);
    FSegment(QObject *parent = nullptr, int video_id = -1, int id = -1,
             QFile *front_mp4 = nullptr, QFile *front_lrv = nullptr, QFile *front_thm = nullptr,
             QFile *back_mp4 = nullptr, QFile *back_lrv = nullptr, QFile *back_thm = nullptr, QFile *back_wav = nullptr);

    bool verify();

    QString toString();

private:
    int video_id;
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
