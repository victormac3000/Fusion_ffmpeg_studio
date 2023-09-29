#include "worker.h"

Worker::Worker(QObject *parent)
    : QObject{parent}
{
    qDebug() << "Worker thread started";
}

Worker::~Worker()
{
    qDebug() << "Worker thread destroyed";
}

void Worker::createProject(QString dcimPath, QString projectName, QString projectPath)
{
    // Check that the DCIM folder is readable

    if (!QFileInfo(dcimPath).isReadable()) {
        emit loadProjectError("Could not read the DCIM folder");
        qWarning() << "DCIM folder invalid, not readable" << dcimPath
                   << "The permissions on the folder are " << QFileInfo(dcimPath).permissions();
        return;
    }

    // Check that the project folder does not exists and try to create one

    if (QFile::exists(projectPath)) {
        if (!QDir(projectPath).isEmpty()) {
            emit loadProjectError("The project folder must be empty");
            qWarning() << "Project folder invalid, not empty" << projectPath;
            return;
        }
    } else {
        if (!QDir().mkpath(projectPath)) {
            emit loadProjectError("Could not create the project folder");
            qWarning() << "Could not create the project folder, mkdir failed on" << projectPath;
            return;
        }
    }

    QDir projectFolder(projectPath);
    QDir dcimFolder(dcimPath);

    if ((!projectFolder.mkdir("DFSegments")) ||
        (!projectFolder.mkdir("DFVideos")) ||
        (!projectFolder.mkdir("DFLowSegments")) ||
        (!projectFolder.mkdir("DFLowVideos")) ||
        (!projectFolder.mkdir("EVideos")) ||
        (!projectFolder.mkdir("ELowVideos"))) {
        emit loadProjectError("Could not create the project subfolders");
        qWarning() << "Could not mkdir the project required folders in" << projectFolder.absolutePath();
        return;
    }

    QFile projectFile(projectPath + "/project.ffs");

    if (!projectFile.open(QFile::ReadWrite)) {
        emit loadProjectError("Could not create the project file");
        qWarning() << "Could not open (create) the project file in" << projectFile.fileName();
        return;
    }

    projectFile.close();

    /*

    QJsonDocument doc;
    QJsonObject mainObj;

    QJsonObject info;

    info.insert("version", QJsonValue::fromVariant(QCoreApplication::applicationVersion()));
    info.insert("dcim", QJsonValue::fromVariant(proposedDCIMFolder));

    mainObj.insert("info", info);
    mainObj.insert("videos", QJsonArray());

    doc.setObject(mainObj);

    int written = projectFile.write(doc.toJson(QJsonDocument::Indented));
    if (written < 0) {
        Dialogs::warning("Could not create the project file");
        qWarning() << "Could not write project file, bytes written" << written << "into file" << projectFile.fileName();
        return;
    }


*/
}

void Worker::loadProject(QString projectPath)
{
/*
    emit loadProjectError("Test error");
    return;


    Project *project = new Project(nullptr, dcimPath);

    if (!project->isValid()) {
        emit loadProjectError("Error opening the project file, the project file may be corrupt");
        return;
    }

    QList<FVideo*> videos = project->getVideos();

    if (!videos.isEmpty()) {
        emit loadProjectUpdate(100, "Indexing DCIM folder from project");
    }

    if (videos.isEmpty()) {
        // Generate videos
        emit loadProjectUpdate(0, "Checking DCIM folder");

        if (!QDir(project->getDcim()).exists()) {
            emit loadProjectError("Cannot open the DCIM folder");
            // TODO Pick new DCIM folder
            qWarning() << "Could not open the DCIM foler, does not exist: " << project->getDcim();
            return;
        }

        QDir front(project->getDcim() + "/100GFRNT");
        QDir back(project->getDcim() + "/100GBACK");

        if (!front.exists()) {
            emit loadProjectError("The DCIM folder has an invalid structure");
            qWarning() << "Could not found the 100GFRNT folder inside the DCIM folder specified: " << project->getDcim();
            return;
        }

        if (!back.exists()) {
            emit loadProjectError("The DCIM folder has an invalid structure");
            qWarning() << "Could not found the 100GBACK folder inside the DCIM folder specified: " << project->getDcim();
            return;
        }

        emit loadProjectUpdate(0, "Indexing front folder files");

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
                    video, segId,
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

            if (!video->verify()) break;

            videos.append(video);
        }
    }

    project->setVideos(videos);
    emit loadProjectFinished(project);
*/
}

void Worker::segmentComplete()
{
    doneSegments++;
    float percent = ((float) doneSegments / (float) totalSegments)*100;
    qDebug() << "New segment complete: " << doneSegments << " of " << totalSegments << " for a percentage of " << QString::number(percent);
    emit loadProjectUpdate(percent);
}

void Worker::videoVerifyError(QString error)
{
    emit loadProjectError(error);
}
