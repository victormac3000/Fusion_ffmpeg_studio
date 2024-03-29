#include "windows/mainwindow.h"
#include "utils/logger.h"
#include "utils/settings.h"

#include <QApplication>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QQuickStyle::setStyle("Imagine");

    a.setApplicationVersion("0.0.1");
    a.setOrganizationName("Victor Corporation");
    a.setApplicationName("Fusion FFmpeg Studio");

    qInstallMessageHandler(Logger::messageHandler);
    Settings::setup();

    MainWindow w;
    w.show();

    return a.exec();
}
