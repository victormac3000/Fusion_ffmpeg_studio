#include "utils/logger.h"
#include "utils/settings.h"
#include "utils/maincontroller.h"

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
        QStringLiteral("mainController"),
        &mainController
    );

    qmlRegisterType<MainController>("FusionFFmpegStudio", 1, 0, "MainController");

    // REGISTER MODEL CLASSES

    const QUrl url(QStringLiteral("qrc:/Qml/Main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &a,
        [url, &tmr](QObject *obj, const QUrl &objUrl)
        {
            if (objUrl != url)
                return;

            qDebug() << "Loading the main window log took"
                     << tmr.elapsed()
                     << "ms";

            if (!obj)
                QCoreApplication::exit(-1);
        }
    );

    engine.load(url);

    return a.exec();
}
