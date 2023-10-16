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
    Project *project = new Project(this, true, projectPath, dcimPath, projectName);

    if (project->isValid()) {
        emit loadProjectFinished(project);
    } else {
        emit loadProjectError("Could not create the project, see the logs for more information");
    }
}

void Worker::loadProject(QString projectPath)
{
    Project *project = new Project(this, false, projectPath);

    if (project->isValid()) {
        emit loadProjectFinished(project);
    } else {
        emit loadProjectError("Could not load the project, see the logs for more information");
    }
}
