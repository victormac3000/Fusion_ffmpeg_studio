#include "worker.h"

Worker::Worker(QObject *parent)
    : QObject{parent}
{
    qDebug() << "Worker thread started";
}

Worker::~Worker()
{
    qDebug() << "Worker thread destroyed";
}

void Worker::createProject(QString dcimPath, QString projectName, QString projectPath)
{
    Project *project = new Project(true, projectPath, dcimPath, projectName);

    connect(project, SIGNAL(loadProjectUpdate(int, QString)), this, SIGNAL(loadProjectUpdate(int,QString)));
    connect(project, SIGNAL(loadProjectError(QString)), this, SIGNAL(loadProjectError(QString)));

    if (project->isValid()) {
        emit loadProjectFinished(project);
    } else {
        emit loadProjectError("Could not create the project, see the logs for more information");
    }
}

void Worker::loadProject(QString projectPath)
{
    Project *project = new Project(false, projectPath);

    connect(project, SIGNAL(loadProjectUpdate(int, QString)), this, SIGNAL(loadProjectUpdate(int,QString)));
    connect(project, SIGNAL(loadProjectError(QString)), this, SIGNAL(loadProjectError(QString)));

    if (project->isValid()) {
        emit loadProjectFinished(project);
    } else {
        emit loadProjectError("Could not load the project, see the logs for more information");
    }
}
