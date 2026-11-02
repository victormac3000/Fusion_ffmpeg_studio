#include "welcomecontroller.h"
#include "models/db/localdb.h"

#include <QDebug>

WelcomeController::WelcomeController(QObject *parent)
    : BaseController{parent}
{
    qDebug() << "Created WelcomeController";
}

void WelcomeController::loadRecentProject(const QJSValue &args, const QJSValue &outputCallback, const QJSValue &errorCallback)
{
    this->task(
        args, outputCallback, errorCallback,
        [](const QVariantMap &args) {

            return args;
        },
        [this](const QVariantMap &result,
               const QJSValue &outputCallback) {
            m_appController->navigateToM(MainController::Pane::Loading, result);

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
                recentProjectsList.append(QVariantMap{
                    {"uuid", recentProject.uuid},
                    {"name", recentProject.name},
                    {"path", recentProject.path}
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

void WelcomeController::onLoadProjectClicked()
{
    qDebug() << "Not yet implemented";
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
