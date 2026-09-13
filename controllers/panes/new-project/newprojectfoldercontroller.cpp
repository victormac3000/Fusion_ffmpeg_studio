#include "newprojectfoldercontroller.h"
#include "utils/settings.h"
#include "utils/toolbox.h"
#include "utils/exceptions/newprojectexception.h"

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

void NewProjectFolderController::validateProjectPath(const QJSValue &args,
                                                const QJSValue &outputCallback,
                                                const QJSValue &errorCallback)
{
    this->task(
        args, outputCallback, errorCallback,
        [](const QVariantMap &args) {
            if (!args.contains("newPath")) {
                throw NewProjectException(
                    "setProjectPath must provide a newPath argument",
                    "Internal error. View logs for more information"
                    );
            }
            QString newPath = args.value("newPath").toString();
            if (newPath.isEmpty()) {
                throw NewProjectException(
                    "newPath argument empty",
                    "Internal error. View logs for more information"
                    );
            }
            QFileInfo projectPathInfo(Toolbox::toLocalPath(newPath));
            if (!projectPathInfo.exists() || !projectPathInfo.isWritable()) {
                throw NewProjectException(
                    "newPath argument must exist and be a writable directory",
                    "You must select a folder in which you can create files"
                    );
            }
            return QVariantMap{
                {"newPath", Toolbox::toLocalPath(newPath)}
            };
        },
        [](const QVariantMap &result,
           const QJSValue &outputCallback) {

            outputCallback.call({
                QJSValue(result.value("newPath").toString())
            });
        }
    );
}

void NewProjectFolderController::validateDCIMPath(const QJSValue &args, const QJSValue &outputCallback, const QJSValue &errorCallback)
{
    this->task(
        args, outputCallback, errorCallback,
        [](const QVariantMap &args) {
            if (!args.contains("newPath")) {
                throw NewProjectException(
                    "setDCIMPath must provide a newPath argument",
                    "Internal error. View logs for more information"
                    );
            }
            QString newPath = args.value("newPath").toString();
            if (newPath.isEmpty()) {
                throw NewProjectException(
                    "newPath argument empty",
                    "Internal error. View logs for more information"
                    );
            }
            QFileInfo projectPathInfo(Toolbox::toLocalPath(newPath));
            if (!projectPathInfo.exists() || !projectPathInfo.isReadable()) {
                throw NewProjectException(
                    "newPath argument must exist and be a readable folder",
                    "You must select a folder with files"
                    );
            }
            return QVariantMap{
                {"newPath", Toolbox::toLocalPath(newPath)}
            };
        },
        [](const QVariantMap &result,
           const QJSValue &outputCallback) {

            outputCallback.call({
                QJSValue(result.value("newPath").toString())
            });
        }
    );
}