#include "windows/mainwindow.h"
#include "utils/logger.h"

#include <QApplication>

void setupSettings();

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationVersion("0.0.1");
    QCoreApplication::setOrganizationName("Victor Corporation");
    QCoreApplication::setApplicationName("Fusion FFmpeg Studio");

    qInstallMessageHandler(Logger::debugHandler);
    setupSettings();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

void setupSettings()
{
    QSettings settings;

    // Set default project path

    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString newDocumentsPath;

    for (int i=1; i<=1000; i++) {
        newDocumentsPath = documentsPath + "/ffs_project_" + QString::number(i);
        if (!QDir().exists(newDocumentsPath)) {
            settings.setValue("defaultProjectPath", newDocumentsPath);
            break;
        }
    }

    // Set default appData path

    if (!settings.contains("appData") || settings.value("appData").toString().isEmpty()) {
        settings.setValue("appData", QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    }

    // Set default output dir

    if (!settings.contains("renderedDir") || settings.value("renderedDir").toString().isEmpty()) {
        QString moviesPath = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
        if (!QDir(moviesPath).mkdir("Fusion FFmpeg Studio Renders")) {
            qWarning() << "Cannot set the default render directory:" << moviesPath;
        }
        settings.setValue("renderedDir", moviesPath + "/Fusion FFmpeg Studio Renders");
    }
}
