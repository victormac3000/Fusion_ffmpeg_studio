#include "preferencescontroller.h"
#include "utils/exceptions/settingsexception.h"
#include "utils/exceptions/preferencescontrollerexception.h"
#include "utils/settings.h"
#include "utils/copier.h"

#include <QGuiApplication>
#include <QVariant>
#include <QFileDialog>
#include <QSettings>
#include <QThread>
#include <QThreadPool>

PreferencesController::PreferencesController(QObject *parent)
    : BaseController{parent}
{}

// Getters

QString PreferencesController::getAppDataPath() const
{
    return Settings::getAppDataPath();
}

QString PreferencesController::getDefaultProjectName() const
{
    return Settings::getDefaultProjectName();
}

QString PreferencesController::getDefaultProjectPath() const
{
    return Settings::getDefaultProjectPath();
}

// Setters

void PreferencesController::setAppDataPath(const QJSValue& args,
                                           const QJSValue& outputCallback,
                                           const QJSValue& errorCallback)
{
    this->backgroundTask(args, outputCallback, errorCallback,
        [](const QVariantMap& args) {
            if (!args.contains("newPath")) {
                throw PreferencesControllerException(
                    "setAppDataPath must provide a newPath argument",
                    "Internal error. View logs for more information"
                );
            }
            QString newPath = args.value("newPath").toString();
            if (newPath.isEmpty()) {
                throw PreferencesControllerException(
                    "newPath argument empty",
                    "Internal error. View logs for more information"
                );
            }

            Settings::setAppDataPath(newPath);
            QThread::sleep(2);

            return QVariantMap{
                {"newPath", Settings::getAppDataPath()}
            };
         },
         [](const QVariantMap &result,
            const QString &error,
            const QJSValue &output,
            const QJSValue &errorCallback) {

            if (!error.isEmpty()) {
                errorCallback.call({QJSValue("Could not change the default aplication data path\n" + error)});
                return;
            }

            output.call({
                QJSValue(result["newPath"].toString())
            });
         }
    );
}

void PreferencesController::setDefaultProjectName(QString newName)
{
    Settings::setDefaultProjectName(newName);
}

void PreferencesController::setDefaultProjectPath(const QJSValue &args,
                                                  const QJSValue &outputCallback,
                                                  const QJSValue &errorCallback)
{
    this->task(
        args, outputCallback, errorCallback,
        [](const QVariantMap &args) {
            if (!args.contains("newPath")) {
                throw PreferencesControllerException(
                    "setDefaultProjectPath must provide a newPath argument",
                    "Internal error. View logs for more information"
                    );
            }
            QString newPath = args.value("newPath").toString();
            if (newPath.isEmpty()) {
                throw PreferencesControllerException(
                    "setDefaultProjectPath argument empty",
                    "Internal error. View logs for more information"
                    );
            }
            Settings::setDefaultProjectPath(newPath);
            return QVariantMap{};
        },
        [](const QVariantMap &result,
           const QJSValue &outputCallback) {

            outputCallback.call({
                QJSValue(Settings::getDefaultProjectPath())
            });
        }
    );
}

// Resetters

void PreferencesController::resetAppDataPath(const QJSValue& args,
                                             const QJSValue& outputCallback,
                                             const QJSValue& errorCallback)
{
    this->backgroundTask(args, outputCallback, errorCallback,
        [](const QVariantMap&) {
            Settings::resetAppDataPath();
            QThread::sleep(2);
            return QVariantMap{};
         },
        [](const QVariantMap &result,
            const QString &error,
            const QJSValue &output,
            const QJSValue &errorCallback) {

            if (!error.isEmpty()) {
                errorCallback.call({QJSValue("Could not reset the default aplication data path.\n" + error)});
                return;
            }

            output.call({});
            QGuiApplication::exit(1);
        }
    );
}

void PreferencesController::resetDefaultProjectPath(const QJSValue &args, const QJSValue &outputCallback, const QJSValue &errorCallback)
{
    this->task(
        args, outputCallback, errorCallback,
        [](const QVariantMap&) {
            Settings::resetDefaultProjectPath();
            return QVariantMap{};
        },
        [](const QVariantMap &result,
           const QJSValue &outputCallback) {

            outputCallback.call({
                QJSValue(Settings::getDefaultProjectPath())
            });
        }
    );
}