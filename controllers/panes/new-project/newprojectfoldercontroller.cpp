#include "newprojectfoldercontroller.h"
#include "utils/settings.h"
#include "utils/toolbox.h"

#include <QUrl>
#include <QFile>
#include <QFileInfo>

NewProjectFolderController::NewProjectFolderController(QObject *parent)
    : BaseController{parent}
{}

QString NewProjectFolderController::getDefaultProjectPath()
{
    return Settings::getDefaultProjectPath();
}

bool NewProjectFolderController::verifyProjectPath(QString projectPath)
{
    try {
        QString path = Toolbox::toLocalPath(projectPath);
        QFileInfo pathInfo(path);
        if (pathInfo.exists()) {
            return false;
        }
        return true;
    } catch (...) {
        return false;
    }
}