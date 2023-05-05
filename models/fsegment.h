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

    QString getIdString();


    QString toString();

    bool verify();

    QFile *getFrontMP4() const;

    QFile *getFrontLRV() const;
    QFile *getFrontTHM() const;
    QFile *getBackMP4() const;
    QFile *getBackLRV() const;
    QFile *getBackTHM() const;
    QFile *getBackWAV() const;

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
