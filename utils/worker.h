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
    void run();

signals:
    void loadProjectUpdate(LoadingProgress progress);
    void loadProjectFinished(Project *project);
    void loadProjectError(QString error);

private slots:

private:
    LoadingInfo loadingInfo;
    Project* project = nullptr;

    void createProject();
    void loadProject();

};

#endif // WORKER_H
