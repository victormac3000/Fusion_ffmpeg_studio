#include "worker.h"
#include "models/project.h"

Worker::Worker(QObject *parent, LoadingInfo loadingInfo)
    : QThread{parent}
{
    this->loadingInfo = loadingInfo;
    this->project = new Project();
    connect(project, SIGNAL(loadProjectUpdate(LoadingProgress)), this, SIGNAL(loadProjectUpdate(LoadingProgress)));
    connect(project, SIGNAL(loadProjectError(QString)), this, SIGNAL(loadProjectError(QString)));
}

Worker::~Worker()
{
    qDebug() << "Worker thread destroyed";
}

void Worker::run()
{
    if (loadingInfo.type == CREATE_PROJECT_FOLDER ||
        loadingInfo.type == CREATE_PROJECT_SD) {
        createProject();
        return;
    }
    if (loadingInfo.type == LOAD_PROJECT) {
        loadProject();
        return;
    }
    qWarning() << "Worker LoadingInfo got an unknown type:" << loadingInfo.type;
    emit loadProjectError("General error with the loader");
}

void Worker::createProject()
{
    project->create(loadingInfo);

    if (project->isValid()) {
        emit loadProjectFinished(project);
    } else {
        emit loadProjectError("Could not create the project");
    }
}

void Worker::loadProject()
{
    project->load(loadingInfo);

    if (project->isValid()) {
        emit loadProjectFinished(project);
    } else {
        emit loadProjectError("Could not load the project");
    }
}

void Worker::segmentComplete()
{

}
