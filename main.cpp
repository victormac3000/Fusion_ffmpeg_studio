#include "windows/mainwindow.h"
#include "utils/logger.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Victor Corporation");
    QCoreApplication::setApplicationName("Fusion FFmpeg Studio");

    QSettings settings;

    qInstallMessageHandler(Logger::debugHandler);

    if (!settings.contains("workingDir") || settings.value("workingDir").toString().isEmpty()) {
        settings.setValue("workingDir", QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    }

    if (!settings.contains("renderedDir") || settings.value("renderedDir").toString().isEmpty()) {
        QString moviesPath = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
        if (!QDir(moviesPath).mkdir("Fusion FFmpeg Studio Renders")) {
            qWarning() << "Cannot set the default render directory:" << moviesPath;
        }
        settings.setValue("renderedDir", moviesPath + "/Fusion FFmpeg Studio Renders");
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    qDebug() << "Mainwindow displayed";
    return a.exec();
}
