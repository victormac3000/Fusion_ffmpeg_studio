#include "preferencescontroller.h"
#include "utils/dialogs.h"
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

bool PreferencesController::changeAppDataDir(QString proposedAppDataDir) const
{
    QString copyAppDataErr = copyAppData(proposedAppDataDir);

    if (!copyAppDataErr.isEmpty()) {
        qWarning() << "Could not change the appData directory: " << copyAppDataErr
                   << "Failed to copy appdata contents to the new path: " << copyAppDataErr;
        return false;
    }

    QSettings().setValue("appData", proposedAppDataDir);
    return true;
}

QString PreferencesController::copyAppData(QString path) const
{
    QString appDataPath = Settings::getAppDataPath();
    QUrl pathURL(path);

    if (!pathURL.isLocalFile()) {
        qWarning() << "Could not move appdata to a non local directory";
        return "You must select a local directory";
    }

    QDir dir(appDataPath);
    QDir newDir(pathURL.toLocalFile());

    if (!dir.exists() || !dir.isReadable()) {
        qCritical() << "Actual appdata does not exist";
        return "Critical error occurred";
    }

    if (!newDir.exists() || !QFileInfo(newDir.absolutePath()).isWritable()) {
        qWarning() << "Could not move appdata to a non writable directory";
        return "You must select a writable directory";
    }

    if (!newDir.isEmpty()) {
        return "You must select an empty directory";
    }

    QFileInfoList dirFiles = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);

    for (const QFileInfo &dirFileInfo: std::as_const(dirFiles)) {
        QString src = dirFileInfo.absoluteFilePath();
        QString dst = newDir.absolutePath() + "/" + dirFileInfo.fileName();
        if (QFile::exists(dst)) {
            qWarning() << "File exists in destination and shouldnt exist"<< dst;
            return "Error copying the files. View the logs for more information";
        }
        if (!QFile(src).copy(dst)) {
            qWarning() << "Could not copy file" << src << "to" << dst;
            return "Error copying the files. View the logs for more information";
        }
    }

    return "";
}