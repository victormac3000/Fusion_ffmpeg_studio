#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QDir>
#include <QFile>

#include "models/fvideo.h"
#include "models/fsegment.h"
#include "models/project.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    ~Worker();

public slots:
    void createProject(QString dcimPath, QString projectName, QString projectPath);
    void loadProject(QString projectPath);

signals:
    void loadProjectUpdate(int percent = 0, QString message = "");
    void loadProjectFinished(Project *project);
    void loadProjectError(QString error);

private slots:

private:
    int doneSegments = 0;
    int totalSegments = 0;

    void segmentComplete();

    Project* project = nullptr;

};

#endif // WORKER_H
