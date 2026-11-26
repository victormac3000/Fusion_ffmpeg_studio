#include "welcomecontroller.h"
#include "models/db/localdb.h"
#include "utils/exceptions/loadingexception.h"
#include "utils/toolbox.h"

#include <QDebug>
#include <QFile>

WelcomeController::WelcomeController(QObject *parent)
    : BaseController{parent}
{
    qDebug() << "Created WelcomeController";
}

void WelcomeController::loadProject(const QJSValue &args, const QJSValue &outputCallback, const QJSValue &errorCallback)
{
    // JS arguments
    // [operation, projectUUID, projectName, projectPath]

    this->task(
        args, outputCallback, errorCallback,
        [this](const QVariantMap &args) {
            QString uuid = args.value("projectUUID").toString();
            QString path = Toolbox::toLocalPath(args.value("projectPath").toString());

            QVariantMap map = args;

            if (uuid.isEmpty()) {
                QDir dir(path);

                if (!dir.exists()) {
                    throw LoadingException(
                        ("Could not open projectPath: " + path).toStdString(),
                        tr("You must select project folder").toStdString()
                    );
                }

                dir.setNameFilters({"*.db"});
                QFileInfoList dbFiles = dir.entryInfoList(QDir::Files);

                if (dbFiles.empty()) {
                    throw LoadingException(
                        ("Could not find a valid database in project: " + path).toStdString(),
                        tr("You project folder is corrupted. Could not load main database").toStdString()
                    );
                }

                // File database
                ProjectDB db(dbFiles.first().absoluteFilePath());

                QVariantMap projectInfo = db.getProjectInfo();

                map.insert("projectUUID", projectInfo.value("uuid"));
                map.insert("projectName", projectInfo.value("name"));
                map.insert("projectPath", path);
            }

            m_appController->navigateToM(MainController::Pane::Loading, map);

            return QVariantMap{};
        },
        [this](const QVariantMap&,
               const QJSValue &outputCallback) {

            outputCallback.call({});
        }
    );
}

void WelcomeController::getRecentProjects(const QJSValue &args, const QJSValue &outputCallback, const QJSValue &errorCallback)
{
    QQmlEngine* engine = qmlEngine(this);
    this->backgroundTask(
        args, outputCallback, errorCallback,
        [](const QVariantMap&) {
            QVariantList recentProjectsList;

            LocalDB localDB;
            QList<RecentProject> recentProjects = localDB.getRecentProjects();

            for (RecentProject& recentProject: recentProjects) {
                bool projectFolderExists = QDir(recentProject.path).exists();

                recentProjectsList.append(QVariantMap{
                    {"uuid", recentProject.uuid},
                    {"name", recentProject.name},
                    {"path", recentProject.path},
                    {"enabled", projectFolderExists}
                });
            }

            return QVariantMap{
                {"recentProjectsList", recentProjectsList}
            };
        },
        [engine](const QVariantMap &result,
           const QString &error,
           const QJSValue &outputCallback,
           const QJSValue &errorCallback) {

            if (!error.isEmpty()) {
                errorCallback.call({QJSValue("Could not obtain the recent projects\n" + error)});
                return;
            }

            outputCallback.call({
                engine->toScriptValue(result.value("recentProjectsList"))
            });
        }
    );
}

void WelcomeController::deleteRecentProject(const QJSValue &args, const QJSValue &outputCallback, const QJSValue &errorCallback)
{
    QQmlEngine* engine = qmlEngine(this);
    this->backgroundTask(
        args, outputCallback, errorCallback,
        [](const QVariantMap& args) {
            LocalDB localDB;

            localDB.deleteRecentProject(args.value("uuid").toString());

            return QVariantMap{};
        },
        [engine](const QVariantMap &result,
                 const QString &error,
                 const QJSValue &outputCallback,
                 const QJSValue &errorCallback) {

            if (!error.isEmpty()) {
                qWarning() << "Could not delete the recent project from the list" << error;
                errorCallback.call({QJSValue(tr("Could not delete the recent project from the list"))});
                return;
            }

            outputCallback.call({});
        }
    );
}

void WelcomeController::onNewProjectClicked()
{
    m_appController->navigateToM(MainController::Pane::NewProject);
}

void WelcomeController::onAboutClicked()
{
    m_appController->addWindowM(MainController::Window::About, true);
}

void WelcomeController::onSettingsClicked()
{
    m_appController->addWindowM(MainController::Window::Preferences, true);
}
