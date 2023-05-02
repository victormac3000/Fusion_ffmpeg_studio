#include "worker.h"

Worker::Worker(QObject *parent)
    : QObject{parent}
{

}

void Worker::loadDCIM(QDir dcim)
{
    QList<FVideo*> videos;

    QDir front(dcim.absoluteFilePath(".") + QString("/100GFRNT"));
    QDir back(dcim.absoluteFilePath(".") + QString("/100GBACK"));

    if (!front.exists()) {
        emit loadDCIMError("Could not found the 100GFRNT folder inside the DCIM folder");
        qWarning() << "Could not found the 100GFRNT folder inside the DCIM folder specified: " << dcim.absoluteFilePath(".");
        return;
    }
    if (!back.exists()) {
        emit loadDCIMError("Could not found the 100GBACK folder inside the DCIM folder");
        qWarning() << "Could not found the 100GBACK folder inside the DCIM folder specified: " << dcim.absoluteFilePath(".");
        return;
    }

    // Check front struct
    front.setNameFilters({"GPFR????.MP4"});
    QFileInfoList mainFrontSegments = front.entryInfoList(QDir::Files);

    for (const QFileInfo &mainFrontSegment: mainFrontSegments) {
        bool isNumber = false;
        int vid = mainFrontSegment.fileName().mid(4,4).toInt(&isNumber);
        if (!isNumber) {
            // Log that this GPFR is not a number
            break;
        }


        FVideo *video = new FVideo(this, vid);
        // Add main segment
        FSegment *mainSegment = new FSegment(
            this, vid, 0,
            new QFile(front.path() + "/GPFR" + QString::number(vid) + ".MP4"),
            new QFile(front.path() + "/GPFR" + QString::number(vid) + ".LRV"),
            new QFile(front.path() + "/GPFR" + QString::number(vid) + ".THM"),
            new QFile(back.path() + "/GPBK" + QString::number(vid) + ".MP4"),
            new QFile(back.path() + "/GPBK" + QString::number(vid) + ".LRV"),
            new QFile(back.path() + "/GPBK" + QString::number(vid) + ".THM"),
            new QFile(back.path() + "/GPBK" + QString::number(vid) + ".WAV")
        );
        if (!mainSegment->verify()) break;
        video->addSegment(mainSegment);

        front.setNameFilters({"GF??" + QString::number(vid) + ".MP4"});
        QFileInfoList mainSecSegments = front.entryInfoList(QDir::Files);

        for (const QFileInfo &mainSecSegment: mainSecSegments) {
            bool isNumber = false;
            int segId = mainSecSegment.fileName().mid(3,2).toInt(&isNumber);
            if (!isNumber) {
                // Log that this GF segment is not a number
                break;
            }

            FSegment *secSegment = new FSegment(
                this, vid, 0,
                new QFile(front.path() + "/GF" + QString::number(segId) + QString::number(vid) + ".MP4"),
                new QFile(front.path() + "/GF" + QString::number(segId) + QString::number(vid) + ".LRV"),
                new QFile(front.path() + "/GF" + QString::number(segId) + QString::number(vid) + ".THM"),
                new QFile(back.path() + "/GB" + QString::number(segId) + QString::number(vid) + ".MP4"),
                new QFile(back.path() + "/GB" + QString::number(segId) + QString::number(vid) + ".LRV"),
                new QFile(back.path() + "/GB" + QString::number(segId) + QString::number(vid) + ".THM"),
                new QFile(back.path() + "/GB" + QString::number(segId) + QString::number(vid) + ".WAV")
            );
            if (!secSegment->verify()) break;
            video->addSegment(mainSegment);
        }

    }

    emit loadDCIMDone(videos);
}
