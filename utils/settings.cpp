#include "settings.h"

void Settings::setup()
{
    QSettings settings;

    // Set default project path

    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString newDocumentsPath;

    for (int i=1; i<=INT32_MAX; i++) {
        newDocumentsPath = documentsPath + "/ffs_project_" + QString::number(i);
        if (!QDir().exists(newDocumentsPath)) {
            settings.setValue("defaultProjectPath", newDocumentsPath);
            break;
        }
    }
    if (settings.value("defaultProjectPath").toString().isEmpty()) {
        Dialogs::critical("There was an internal error starting the application\nCheck the logs for more information",
                  "Could not find a valid default project path. The documents path is:" + documentsPath);
    }

    // Set default appData path

    if (!settings.contains("appData") || settings.value("appData").toString().isEmpty()) {
        settings.setValue("appData", QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    }

    // Check that binaries exist
}
