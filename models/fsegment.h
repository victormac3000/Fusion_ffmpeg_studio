#ifndef QSEGMENT_H
#define QSEGMENT_H

#include <QObject>
#include <QFile>
#include <QFileInfo>

class FSegment : public QObject
{
    Q_OBJECT
public:
    explicit FSegment(QObject *parent = nullptr, int id = -1, QFile *front_mp4 = nullptr, QFile *front_lrv = nullptr, QFile *front_thm = nullptr,
                      QFile *back_mp4 = nullptr, QFile *back_lrv = nullptr, QFile *back_thm = nullptr, QFile *back_wav = nullptr);

    bool verify();
    QString getIdString();
    QString toString();

signals:
    void verifyError(QString error);

private:
    int id;

    QFile *frontMP4 = nullptr;
    QFile *frontLRV = nullptr;
    QFile *frontTHM = nullptr;

    QFile *backMP4 = nullptr;
    QFile *backLRV = nullptr;
    QFile *backTHM = nullptr;
    QFile *backWAV = nullptr;

};

#endif // QSEGMENT_H
