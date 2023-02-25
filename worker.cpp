#include "worker.h"

Worker::Worker(QObject *parent)
    : QThread{parent}
{

}

void Worker::checkDCIMStructure(QDir dcim)
{
    QFileInfoList dcimFolders = dcim.entryInfoList(QDir::AllDirs);
    bool frontFound = false;
    bool backFound = false;

    for (const QFileInfo &dcimDir: dcimFolders) {
        if (dcimDir.fileName() == "100GFRNT") {
            frontFound = true;
        }
        if (dcimDir.fileName() == "100GBACK") {
            backFound = true;
        }
    }

    if (!frontFound) {
        emit checkDCIMStructureDone(false, "Could not found the 100GFRNT folder");
        return;
    }

    if (!backFound) {
        emit checkDCIMStructureDone(false, "Could not found the 100GFRNT folder");
        return;
    }

    emit checkDCIMStructureDone(true, "");
}
