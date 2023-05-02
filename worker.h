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
    Worker(QObject *parent = nullptr);

public slots:
    void loadDCIM(QDir dcim);

signals:
    void loadDCIMUpdate(int percent);
    void loadDCIMDone(QList<FVideo*> videos = QList<FVideo*>());
    void loadDCIMError(QString error);

private:

};

#endif // WORKER_H
