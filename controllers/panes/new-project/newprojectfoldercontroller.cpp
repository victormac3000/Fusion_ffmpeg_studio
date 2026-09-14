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

QString NewProjectFolderController::getDefaultProjectName()
{
    return Settings::getDefaultProjectName();
}

QString NewProjectFolderController::getDefaultProjectPath()
{
    return Settings::getDefaultProjectPath();
}

void NewProjectFolderController::validateProjectName(const QJSValue &args, const QJSValue &outputCallback, const QJSValue &errorCallback)
{
    this->task(
        args, outputCallback, errorCallback,
        [this](const QVariantMap &args) {
            if (!args.contains("projectName") || !args.contains("projectPath")) {
                throw NewProjectException(
                    "validateProjectName must provide a projectName and projectPath arguments",
                    "Internal error. View logs for more information"
                    );
            }
            QString projectName = args.value("projectName").toString();
            QString projectPath = args.value("projectPath").toString();
            if (projectName.isEmpty()) {
                throw NewProjectException(
                    "projectName argument empty",
                    "A project name is required", true
                );
            }
            if (projectPath.isEmpty()) {
                throw NewProjectException(
                    "projectPath argument empty",
                    "Internal error. View logs for more information"
                );
            }

            QList<QChar> badChars = {'*', '+', '-', '/', '\\', '.'};
            for (const QChar& character: std::as_const(projectName)) {
                if (badChars.contains(character)) {
                    throw NewProjectException(
                        "projectName contains bad characters",
                        "The characters * + - / \\ . are not allowed", true
                    );
                }
            }

            QString projectPathL = Toolbox::toLocalPath(projectPath);


            QPair<QString,QString> genProjectNamePath = generateProjectNamePath(projectName, projectPathL);

            return QVariantMap{
                {"mergedPath", genProjectNamePath.second}
            };
        },
        [](const QVariantMap &result,
           const QJSValue &outputCallback) {

            outputCallback.call({
                QJSValue(result.value("mergedPath").toString()),
            });
        }
        );
}

void NewProjectFolderController::generateProjectPath(const QJSValue &args,
                                                const QJSValue &outputCallback,
                                                const QJSValue &errorCallback)
{
    this->task(
        args, outputCallback, errorCallback,
        [this](const QVariantMap &args) {
            if (!args.contains("projectName")) {
                throw NewProjectException(
                    "generateProjectPath must provide a projectName argument",
                    "Internal error. View logs for more information"
                    );
            }

            if (!args.contains("selectedFolder") || args.value("selectedFolder").toString().isEmpty()) {
                throw NewProjectException(
                    "generateProjectPath must provide a selectedFolder argument",
                    "Internal error. View logs for more information"
                    );
            }

            QString projectName = args.value("projectName").toString();
            QString selectedFolder = args.value("selectedFolder").toString();
            QString selectedFolderL = Toolbox::toLocalPath(selectedFolder);

            QFileInfo selectedFolderInfo(selectedFolderL);
            if (!selectedFolderInfo.exists() || !selectedFolderInfo.isDir() || !selectedFolderInfo.isWritable()) {
                throw NewProjectException(
                    "newPath argument must exist and be a writable directory",
                    "You must select a folder in which you can create files"
                );
            }

            QPair<QString,QString> genProjectNamePath = generateProjectNamePath(projectName, selectedFolderL);

            return QVariantMap{
                {"newBaseFolder", selectedFolderL},
                {"newProjectPath", genProjectNamePath.second}
            };
        },
        [](const QVariantMap &result,
           const QJSValue &outputCallback) {

            outputCallback.call({
                QJSValue(result.value("newBaseFolder").toString()),
                QJSValue(result.value("newProjectPath").toString())
            });
        }
    );
}

void NewProjectFolderController::validateProjectpath(const QJSValue &args,
                                                    const QJSValue &outputCallback,
                                                    const QJSValue &errorCallback)
{
    this->task(
        args, outputCallback, errorCallback,
        [this](const QVariantMap &args) {
            if (!args.contains("projectBasePath") || args.value("projectBasePath").toString().isEmpty()) {
                throw NewProjectException(
                    "validateProjectpath must provide a projectBasePath argument",
                    "Internal error. View logs for more information"
                );
            }
            QString projectBasePath = args.value("projectBasePath").toString();
            QString projectBasePathL = Toolbox::toLocalPath(projectBasePath);

            QString projectName = args.value("projectName").toString();
            QString projectNameGen = generateProjectNamePath(projectName, projectBasePathL).first;

            return QVariantMap{
                {"pathDiffersName", projectName != projectNameGen}
            };
        },
        [](const QVariantMap &result,
           const QJSValue &outputCallback) {
            outputCallback.call({
                QJSValue(result.value("pathDiffersName").toBool())
            });
        }
    );
}

void NewProjectFolderController::validateDCIMPath(const QJSValue &args, const QJSValue &outputCallback, const QJSValue &errorCallback)
{
    this->task(
        args, outputCallback, errorCallback,
        [](const QVariantMap &args) {
            QString dcimPath = args.value("dcimPath").toString();
            QString dcimPathL = Toolbox::toLocalPath(dcimPath);

            if (dcimPath.isEmpty()) {
                throw NewProjectException(
                    "validateDCIMPath must provide a dcimPath argument",
                    "Internal error. View logs for more information"
                );
            }

            QDir dcimDir(dcimPathL);
            if (dcimDir.dirName() != "DCIM") {
                throw NewProjectException(
                    "dcimPath must point to a folder called DCIM",
                    "You must select the DCIM folder with the source videos.\n"
                    "The DCIM folder must contain 100GFRNT and 100GBACK folders, copied manually from each SD card"
                );
            }

            return QVariantMap{
                {"dcimPath", dcimPathL}
            };
        },
        [](const QVariantMap &result,
           const QJSValue &outputCallback) {

            outputCallback.call({
                QJSValue(result.value("dcimPath").toString())
            });
        }
    );
}

void NewProjectFolderController::createProject(const QJSValue &args, const QJSValue &outputCallback, const QJSValue &errorCallback)
{
    this->task(
        args, outputCallback, errorCallback,
        [](const QVariantMap &args) {
            QString projectName = args.value("projectName").toString();
            QString projectPath = args.value("projectPath").toString();
            QString dcimPath = args.value("dcimPath").toString();
            bool dcimCopy = args.value("dcimCopy").toBool();

            return QVariantMap{
                {"projectName", projectName},
                {"projectPath", projectPath},
                {"dcimPath", dcimPath},
                {"dcimCopy", dcimCopy}
            };
        },
        [](const QVariantMap &result,
           const QJSValue &outputCallback) {

            outputCallback.call({
                QJSValue(result.value("projectName").toString()),
                QJSValue(result.value("projectPath").toString()),
                QJSValue(result.value("dcimPath").toString()),
                QJSValue(result.value("dcimCopy").toBool())
            });
        }
    );
}

QPair<QString,QString> NewProjectFolderController::generateProjectNamePath(QString name, QString basePath)
{
    QDir baseProjectFolder(basePath);
    bool validPath = false;
    int i = 0;

    do {
        QString generatedName = (i < 1) ? name : (name + QString::number(i));
        QString generatedNamePath = baseProjectFolder.filePath(generatedName);
        QDir projectFolder(generatedNamePath);

        if (!projectFolder.exists()) {
            return {generatedName, generatedNamePath};
        }

        i++;
    } while (!validPath || i > 1000000);

    qWarning() << "Could not generate project path";
    throw NewProjectException("Could not generate project path");
}