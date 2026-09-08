#include "preferencescontroller.h"
#include "utils/exceptions/settingsexception.h"
#include "utils/settings.h"
#include "utils/copier.h"

#include <QVariant>
#include <QFileDialog>
#include <QSettings>
#include <QThread>
#include <QThreadPool>

PreferencesController::PreferencesController(QObject *parent)
    : BaseController{parent}
{

}

QString PreferencesController::getAppDataPath() const
{
    return Settings::getAppDataPath();
}

QString PreferencesController::getDefaultProjectName() const
{
    return Settings::getDefaultProjectName();
}

void PreferencesController::setDefaultProjectName(QString defaultName)
{
    Settings::setDefaultProjectName(defaultName);
}

void PreferencesController::changeAppDataDir(QString proposedAppDataDir)
{
    QThreadPool::globalInstance()->start([this, proposedAppDataDir]() {
        QVariantMap ret;
        ret.insert("ok", false);

        try {
            Settings::setAppDataPath(proposedAppDataDir);
            QThread::sleep(5);
            ret.insert("ok", true);
            ret.insert("newPath", Copier::toLocalPath(proposedAppDataDir));
        } catch (const SettingsException& e) {
            ret.insert(
                "error", "Could not change the appData directory to\n"
                + Copier::toLocalPath(proposedAppDataDir) + "\n"
                + QString::fromStdString(e.userMessage())
            );
        }

        emit appDataPathChanged(ret);
    });
}

void PreferencesController::resetAppDataDir()
{
    Settings::resetAppDataPath();
}