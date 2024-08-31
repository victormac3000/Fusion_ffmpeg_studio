#include "worker.h"

Worker::Worker(QObject *parent, LoadingInfo loadingInfo)
    : QObject{parent}
{
    qDebug() << "Worker thread started";
    this->loadingInfo = loadingInfo;
    this->project = new Project();
    connect(project, SIGNAL(loadProjectUpdate(int,QString)), this, SIGNAL(loadProjectUpdate(int,QString)));
    connect(project, SIGNAL(loadProjectError(QString)), this, SIGNAL(loadProjectError(QString)));
}

Worker::~Worker()
{
    qDebug() << "Worker thread destroyed";
}

void Worker::work()
{
    if (loadingInfo.type == CREATE_PROJECT_FOLDER) {
        createProjectFolder();
    }
    if (loadingInfo.type == CREATE_PROJECT_SD) {
        createProjectSd();
    }
    if (loadingInfo.type == LOAD_PROJECT) {
        loadProject();
    }
}

void Worker::createProjectFolder()
{
    /*
    project->create(projectPath, dcimPath, projectName);

    if (project->isValid()) {
        emit loadProjectFinished(project);
    } else {
        emit loadProjectError("Could not create the project, see the logs for more information");
    }
*/
}

void Worker::createProjectSd()
{
    /*
    project->create(projectPath, dcimPath, projectName);

    if (project->isValid()) {
        emit loadProjectFinished(project);
    } else {
        emit loadProjectError("Could not create the project, see the logs for more information");
    }
*/
}

void Worker::loadProject()
{
    /*
    project->load(projectPath);

    if (project->isValid()) {
        emit loadProjectFinished(project);
    } else {
        emit loadProjectError("Could not load the project, see the logs for more information");
    }
*/
}

void Worker::segmentComplete()
{

}
