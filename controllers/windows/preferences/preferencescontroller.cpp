#include "preferencescontroller.h"
#include "utils/dialogs.h"
#include "utils/exceptions/settingsexception.h"
#include "utils/settings.h"

#include <QVariant>
#include <QFileDialog>
#include <QSettings>

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

QVariantMap PreferencesController::changeAppDataDir(QString proposedAppDataDir) const
{
    try {
        Settings::setAppDataPath(proposedAppDataDir);
    } catch (const SettingsException& e) {
        return {
            {"ok", false},
            {"error", "Could not change the appData directory to\n"
                    + QUrl(proposedAppDataDir).toLocalFile() + "\n"
                    + QString::fromStdString(e.userMessage())
            }
        };
    }
    return {
        {"ok", true}
    };
}