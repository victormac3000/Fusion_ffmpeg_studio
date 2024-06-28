#include "worker.h"

Worker::Worker(QObject *parent)
    : QObject{parent}
{
    qDebug() << "Worker thread started";
    project = new Project();
    connect(project, SIGNAL(loadProjectUpdate(int,QString)), this, SIGNAL(loadProjectUpdate(int,QString)));
    connect(project, SIGNAL(loadProjectError(QString)), this, SIGNAL(loadProjectError(QString)));
}

Worker::~Worker()
{
    qDebug() << "Worker thread destroyed";
}

void Worker::createProject(QString dcimPath, QString projectName, QString projectPath)
{
    project->create(projectPath, dcimPath, projectName);

    if (project->isValid()) {
        emit loadProjectFinished(project);
    } else {
        emit loadProjectError("Could not create the project, see the logs for more information");
    }
}

void Worker::loadProject(QString projectPath)
{
    project->load(projectPath);

    if (project->isValid()) {
        emit loadProjectFinished(project);
    } else {
        emit loadProjectError("Could not load the project, see the logs for more information");
    }
}
