#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QDir>
#include <QFile>

#include "models/fvideo.h"
#include "models/fsegment.h"
#include "models/project.h"
#include "models/loadinginfo.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr, LoadingInfo loadingInfo = {});
    ~Worker();

public slots:
    void work();

signals:
    void loadProjectUpdate(int percent = 0, QString message = "");
    void loadProjectFinished(Project *project);
    void loadProjectError(QString error);

private slots:

private:
    LoadingInfo loadingInfo;
    int doneSegments = 0;
    int totalSegments = 0;
    Project* project = nullptr;

    void createProjectFolder();
    void createProjectSd();
    void loadProject();

    void segmentComplete();

};

#endif // WORKER_H
