#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QDir>
#include <QFile>

#include "models/fvideo.h"
#include "models/fsegment.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    ~Worker();

public slots:
    void loadDCIM(QString dcimPath);

signals:
    void loadDCIMUpdate(int percent = 0, QString message = "");
    void loadDCIMDone(QList<FVideo*> videos = QList<FVideo*>());
    void loadDCIMError(QString error);

private slots:
    void videoVerifyError(QString error);

private:
    int doneSegments = 0;
    int totalSegments = 0;

    void segmentComplete();

};

#endif // WORKER_H
