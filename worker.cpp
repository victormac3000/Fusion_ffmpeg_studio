#include "worker.h"
#include "models/project.h"

Worker::Worker(QObject *parent, LoadingInfo loadingInfo)
    : QObject{parent}
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
    switch (loadingInfo.type)
    {
        case CREATE_PROJECT_FOLDER:
        case CREATE_PROJECT_SD:
            createProject();
            break;
        case LOAD_PROJECT:
            loadProject();
            break;
        default:
            qWarning() << "Worker LoadingInfo got an unknown type:" << loadingInfo.type;
            emit loadProjectError("General error with the loader");
    }
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
