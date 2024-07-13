#include "windows/mainwindow.h"
#include "utils/logger.h"
#include "utils/settings.h"

#include <QApplication>
#include <QQuickStyle>
#include <QElapsedTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QQuickStyle::setStyle("Imagine");

    a.setApplicationVersion("0.0.1");
    a.setOrganizationName("Victor Corporation");
    a.setApplicationName("Fusion FFmpeg Studio");

    Logger::setup();
    Settings::setup();

    QElapsedTimer tmr;
    tmr.start();

    MainWindow w;
    w.show();

    std::cout << "Loading the main window log took " << tmr.elapsed() << " miliseconds" << std::endl;

    return a.exec();
}
