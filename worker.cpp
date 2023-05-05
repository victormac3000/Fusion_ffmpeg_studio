#include "worker.h"

Worker::Worker(QObject *parent)
    : QObject{parent}
{

}

void Worker::loadDCIM(QString dcimPath)
{
    QList<FVideo*> videos;
    QDir dcim(dcimPath);

    emit loadDCIMUpdate(0, "Checking base folder");
    //QThread::sleep(1);

    if (!dcim.exists()) {
        emit loadDCIMError("Cannot open the selected DCIM folder");
        qWarning() << "Could not open the DCIM foler, does not exist: " << dcimPath;
        return;
    }

    QDir front(dcimPath + QString("/100GFRNT"));
    QDir back(dcimPath + QString("/100GBACK"));

    if (!front.exists()) {
        emit loadDCIMError("Could not found the 100GFRNT folder inside the DCIM folder");
        qWarning() << "Could not found the 100GFRNT folder inside the DCIM folder specified: " << dcimPath;
        return;
    }

    if (!back.exists()) {
        emit loadDCIMError("Could not found the 1RefRefRefRef00GBACK folder inside the DCIM folder");
        qWarning() << "Could not found the 100GBACK folder inside the DCIM folder specified: " << dcimPath;
        return;
    }

    emit loadDCIMUpdate(0, "Checking front folder files");
    //QThread::sleep(1);

    front.setNameFilters({"GPFR????.MP4"});
    QFileInfoList mainFrontSegments = front.entryInfoList(QDir::Files);

    back.setNameFilters({"GPBK????.MP4"});
    QFileInfoList mainBackSegments = back.entryInfoList(QDir::Files);

    totalSegments = mainFrontSegments.length() + mainBackSegments.length();

    for (const QFileInfo &mainFrontSegment: mainFrontSegments) {
        bool isNumber = false;
        int vid = mainFrontSegment.fileName().mid(4,4).toInt(&isNumber);
        if (!isNumber) {
            qWarning() << "Found a main front segment with an invalid ID: " << mainFrontSegment.absoluteFilePath();
            break;
        }

        FVideo *video = new FVideo(nullptr, vid);
        connect(video, SIGNAL(verifyError(QString)), this, SLOT(videoVerifyError(QString)));

        // Add main segment
        FSegment *mainSegment = new FSegment(
            video, 0,
            new QFile(front.path() + "/GPFR" + video->getIdString() + ".MP4"),
            new QFile(front.path() + "/GPFR" + video->getIdString() + ".LRV"),
            new QFile(front.path() + "/GPFR" + video->getIdString() + ".THM"),
            new QFile(back.path() + "/GPBK" + video->getIdString() + ".MP4"),
            new QFile(back.path() + "/GPBK" + video->getIdString() + ".LRV"),
            new QFile(back.path() + "/GPBK" + video->getIdString() + ".THM"),
            new QFile(back.path() + "/GPBK" + video->getIdString() + ".WAV")
        );
        if (!video->addSegment(mainSegment)) return;
        segmentComplete();

        front.setNameFilters({"GF??" + video->getIdString() + ".MP4"});
        QFileInfoList mainSecSegments = front.entryInfoList(QDir::Files);

        for (const QFileInfo &mainSecSegment: mainSecSegments) {
            bool isNumber = false;
            int segId = mainSecSegment.fileName().mid(2,2).toInt(&isNumber);
            if (!isNumber) {
                qWarning() << "Found a secondary front segment with an invalid ID: " << mainFrontSegment.absoluteFilePath();
                break;
            }

            QString segIdString = QString::number(segId);
            while (segIdString.length() < 2) segIdString.insert(0, "0");

            FSegment *secSegment = new FSegment(
                video, 0,
                new QFile(front.path() + "/GF" + segIdString + video->getIdString() + ".MP4"),
                new QFile(front.path() + "/GF" + segIdString + video->getIdString() + ".LRV"),
                new QFile(front.path() + "/GPFR" + video->getIdString() + ".THM"),
                new QFile(back.path() + "/GB" + segIdString + video->getIdString() + ".MP4"),
                new QFile(back.path() + "/GB" + segIdString + video->getIdString() + ".LRV"),
                new QFile(back.path() + "/GPBK" + video->getIdString() + ".THM"),
                new QFile(back.path() + "/GB" + segIdString + video->getIdString() + ".WAV")
            );

            if (!video->addSegment(secSegment)) return;
            segmentComplete();
        }

    }

    emit loadDCIMDone(&videos);
}

void Worker::segmentComplete()
{
    doneSegments++;
    float percent = ((float) doneSegments / (float) totalSegments)*100;
    qDebug() << "New segment complete: " << doneSegments << " of " << totalSegments << " for a percentage of " << QString::number(percent);
    emit loadDCIMUpdate(percent);
}

void Worker::videoVerifyError(QString error)
{
    emit loadDCIMError(error);
}
