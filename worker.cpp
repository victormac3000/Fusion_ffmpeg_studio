#include "worker.h"
#include "models/project.h"

Worker::Worker(QObject *parent, LoadingInfo loadingInfo)
    : QObject{parent}
{
    qDebug() << "Worker thread started";
    this->loadingInfo = loadingInfo;
    this->project = new Project();
    connect(project, SIGNAL(loadProjectUpdate(LoadingProgress)), this, SIGNAL(loadProjectUpdate(LoadingProgress)));
    connect(project, SIGNAL(loadProjectError(QString)), this, SIGNAL(loadProjectError(QString)));
}

Worker::~Worker()
{
    qDebug() << "Worker thread destroyed";
}

void Worker::work()
{
    if (loadingInfo.type == CREATE_PROJECT_FOLDER ||
        loadingInfo.type == CREATE_PROJECT_SD) {
        createProject();
    }
    if (loadingInfo.type == LOAD_PROJECT) {
        loadProject();
    }
}

void Worker::createProject()
{
    project->create(loadingInfo);

    if (project->isValid()) {
        emit loadProjectFinished(project);
    } else {
        emit loadProjectError("Could not create the project, see the logs for more information");
    }
}

void Worker::loadProject()
{
    project->load(loadingInfo.projectPath);

    if (project->isValid()) {
        emit loadProjectFinished(project);
    } else {
        emit loadProjectError("Could not load the project, see the logs for more information");
    }
}

void Worker::segmentComplete()
{

}
