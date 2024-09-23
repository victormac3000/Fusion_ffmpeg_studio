#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QDir>
#include <QFile>

#include "models/loading.h"

struct LoadingProgress;
class Project;

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr, LoadingInfo loadingInfo = {});
    ~Worker();

public slots:
    void work();

signals:
    void loadProjectUpdate(LoadingProgress progress);
    void loadProjectFinished(Project *project);
    void loadProjectError(QString error);

private slots:

private:
    LoadingInfo loadingInfo;
    int doneSegments = 0;
    int totalSegments = 0;
    Project* project = nullptr;

    void createProject();
    void loadProject();

    void segmentComplete();

};

#endif // WORKER_H
