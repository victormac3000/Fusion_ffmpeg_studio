#include "utils/logger.h"
#include "utils/settings.h"
#include "controllers/maincontroller.h"
#include "controllers/panes/new-project/newprojectcontroller.h"
#include "controllers/panes/welcome/welcomecontroller.h"
#include "controllers/windows/about/aboutcontroller.h"

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
    qmlRegisterType<WelcomeController>("FusionFFmpegStudio", 1, 0, "WelcomeController");
    qmlRegisterType<AboutController>("FusionFFmpegStudio", 1, 0, "AboutController");

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
