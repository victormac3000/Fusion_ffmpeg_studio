#include "utils/logger.h"
#include "utils/settings.h"

#include "controllers/maincontroller.h"
#include "controllers/panes/new-project/newprojectcontroller.h"
#include "controllers/panes/new-project/newprojectsdcontroller.h"
#include "controllers/panes/new-project/newprojectfoldercontroller.h"
#include "controllers/panes/welcomecontroller.h"
#include "controllers/windows/aboutcontroller.h"
#include "controllers/windows/preferencescontroller.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QElapsedTimer>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    QQuickStyle::setStyle("Imagine");

    a.setApplicationVersion("0.0.1");
    a.setOrganizationName("Victor Corporation");
    a.setApplicationName("Fusion FFmpeg Studio");

    Logger::setup();
    Settings::setup();

    QElapsedTimer tmr;
    tmr.start();

    QQmlApplicationEngine engine;
    MainController mainController;

    engine.rootContext()->setContextProperty(
        "mainController",
        &mainController
    );

    qmlRegisterType<NewProjectController>("FusionFFmpegStudio", 1, 0, "NewProjectController");
    qmlRegisterType<NewProjectFolderController>("FusionFFmpegStudio", 1, 0, "NewProjectFolderController");
    qmlRegisterType<NewProjectSdController>("FusionFFmpegStudio", 1, 0, "NewProjectSdController");
    qmlRegisterType<WelcomeController>("FusionFFmpegStudio", 1, 0, "WelcomeController");
    qmlRegisterType<AboutController>("FusionFFmpegStudio", 1, 0, "AboutController");
    qmlRegisterType<PreferencesController>("FusionFFmpegStudio", 1, 0, "PreferencesController");

    // REGISTER MODEL CLASSES

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &a,
        [&tmr](QObject *obj, const QUrl &url)
        {
            qDebug() << "Loading the main window took"
                     << tmr.elapsed()
                     << "ms";

            if (!obj)
                QCoreApplication::exit(-1);
        }
    );

    engine.loadFromModule("FusionFFmpegStudio", "Main");

    return a.exec();
}
