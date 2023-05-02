#include "windows/mainwindow.h"
#include "utils/logger.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Victor Corporation");
    QCoreApplication::setApplicationName("Fusion FFmpeg Studio");
    qInstallMessageHandler(Logger::debugHandler);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    qDebug() << "Mainwindow displayed";
    return a.exec();
}
