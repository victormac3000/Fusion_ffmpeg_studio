#include "project.h"
#include "fsegment.h"
#include "fvideo.h"
#include "loading.h"
#include "utils/exceptions/DBException.h"
#include "utils/settings.h"
#include "utils/exceptions/projectexception.h"

#include <QElapsedTimer>

Project::Project(QObject *parent)
    : QObject{parent}
{
    this->localDBManager = new LocalDB(Settings::getLocalDBPath());
}

Project::~Project()
{
    delete dbManager;
    delete localDBManager;
}

void Project::create(LoadingInfo info)
{
    qDebug() << "Project thread" << QThread::currentThreadId();

    try {
        this->loadingInfo = info;
        this->loadingProgress = LoadingProgress{};
        this->dcimLinked = !this->loadingInfo.copyDCIM;

        this->loadingProgress.stepCount = this->dcimLinked ? 2 : 3;

        bool lInfoTrace = false;

        if (lInfoTrace) {
            qDebug() << "-----LOADING INFORMATION START-----";
            qDebug() << "type" << loadingInfo.type;
            qDebug() << "projectPath" << loadingInfo.projectPath;
            qDebug() << "projectName" << loadingInfo.projectName;
            qDebug() << "dcimPath" << loadingInfo.dcimPath;
            qDebug() << "frontVolumePath" << loadingInfo.frontVolumePath;
            qDebug() << "backVolumePath" << loadingInfo.backVolumePath;
            qDebug() << "copyDCIM" << loadingInfo.copyDCIM;
            qDebug() << "------LOADING INFORMATION END-------";
        }

        switch (loadingInfo.type) {
            case CREATE_PROJECT_FOLDER:
                this->createProjectFolder();
                break;
            case CREATE_PROJECT_SD:
                this->createProjectSD();
                break;
            case LOAD_PROJECT:
                this->loadProject();
                break;
            default:
                throw ProjectException(
                    "Failed to create/load project: operation unknown (" +
                    std::to_string(loadingInfo.type) + ")"
                );
        }

        QStringList badVideosIDs;
        if (!this->badVideos.isEmpty()) {
            for (QString& videoID: this->badVideos.keys()) {
                badVideosIDs.append(videoID);
                qWarning() << "Could not index video " + videoID + ": " + this->badVideos.value(videoID);
            }
        }

        emit loadProjectDone(LoadingDone {
            .badVideos = badVideosIDs,
            .numVideos = (int) this->videos.length()
        });
    } catch (CustomException& e) {
        qWarning() << "Error creating or loading project:" << e.what();

        QString userError = QString::fromStdString(e.userMessage());

        if (userError.isEmpty()) {
            if (this->loadingInfo.type == LOAD_PROJECT) {
                userError = "There was an unknwon error loading the project";
            } else {
                userError = "There was an unknwon error creating the project";
            }
        }

        emit loadProjectError({
            .message = userError,
            .progress = this->loadingProgress
        });
    }
}

void Project::getVideos()
{
    emit videosReady(this->videos);
}

void Project::createProjectFolder()
{
    if (this->dcimLinked) {
        this->dcim = this->loadingInfo.dcimPath;
    }

    this->createProjectTGenerate();

    if (!this->dcimLinked) {
        this->createProjectTCopyDCIM();
    }

    this->indexVideos();
    this->save();
}

void Project::createProjectSD()
{
    this->createProjectTGenerate();

    if (!this->dcimLinked) {
        this->createProjectTCopyDCIM();
    }

    this->indexVideos();
    this->save();
}

void Project::loadProject()
{
    this->loadingProgress.stepID = READ_PROJECT_DIR;
    this->loadingProgress.stepCount = 2;
    this->loadingProgress.stepNumber = 0;
    emit loadProjectUpdate(this->loadingProgress);

    if (this->loadingInfo.projectName.isEmpty()) {
        throw ProjectException("projectName empty");
    }

    if (this->loadingInfo.projectPath.isEmpty()) {
        throw ProjectException("projectPath empty");
    }

    if (!QDir(this->loadingInfo.projectPath).exists()) {
        throw ProjectException("projectPath does not exist");
    }

    if (this->loadingInfo.projectUUID.isEmpty()) {
        throw ProjectException("projectUUID empty");
    }

    this->uuid = this->loadingInfo.projectUUID;
    this->dir = QDir(this->loadingInfo.projectPath);
    this->name = this->loadingInfo.projectName;
    this->dbManager = new ProjectDB(this->dir.filePath(this->name)+".db");

    QVariantMap projectInfo = this->dbManager->getProjectInfo();
    if (projectInfo.isEmpty()) {
        throw ProjectException(
            "Could not load project database",
            "The database of the project is corrupted"
        );
    }

    this->checkVersions(projectInfo);

    QString dcimPath;
    bool dcimLinked = projectInfo.value("dcim_linked").toBool();

    if (dcimLinked) {
        dcimPath = projectInfo.value("dcim_path").toString();
        if (dcimPath.isEmpty() || !QDir(dcimPath).exists()) {
            throw ProjectException(
                "Could not find valid DCIM linked folder",
                "The linked DCIM folder to this project is not found"
            );
        }
    } else {
        dcimPath = this->dir.filePath("DCIM");
        if (!QDir(dcimPath).exists()) {
            throw ProjectException(
                "Could not find the DCIM folder of the project",
                "The project files are corrupted"
            );
        }
    }

    this->dcim = dcimPath;

    QString frontPath = this->dcim.filePath("100GFRNT");
    QString backPath = this->dcim.filePath("100GBACK");

    if (!QDir(frontPath).exists() || !QDir(backPath).exists()) {
        throw ProjectException(
            "Could not find 100GFRNT or 100GBACK folders on DCIM folder",
            "The project files are corrupted"
        );
    }

    this->frontDir = QDir(frontPath);
    this->backDir = QDir(backPath);

    this->indexVideos();
    this->save();
}

void Project::checkVersions(QVariantMap projectInfo)
{
    QList<int> appVersionNumbers = Settings::getAppVersionNumbers();
    int v_major = projectInfo.value("version_major").toInt();
    int v_mid = projectInfo.value("version_mid").toInt();
    int v_minor = projectInfo.value("version_minor").toInt();

    QString appVersion = QString::number(appVersionNumbers.at(0)) + "." +
                         QString::number(appVersionNumbers.at(1)) + "." +
                         QString::number(appVersionNumbers.at(2));
    QString projectVersion = QString::number(v_major) + "." +
                             QString::number(v_mid) + "." +
                             QString::number(v_minor);

    if (v_major <= appVersionNumbers.at(0) ||
        v_mid <= appVersionNumbers.at(1) ||
        v_minor <= appVersionNumbers.at(2)) {
        // Upgrade
        qInfo() << QString("Project upgraded from version %1 to %2").arg(projectVersion, appVersion);
    } else {
        // Downgrade (NOT SUPPORTED)
        throw ProjectException(
            QString("Project version (%1) is newer than app version (%2)").arg(projectVersion, appVersion).toStdString(),
            QString("The project was generated with a newer version of the application. Update %1 to at least version %2")
                .arg(QCoreApplication::applicationName(), projectVersion)
                .toStdString()
        );
    }
}

void Project::createProjectTGenerate()
{
    this->loadingProgress.stepID = GENERATE_PROJECT_DIRS;
    this->loadingProgress.stepNumber = 0;
    emit loadProjectUpdate(this->loadingProgress);

    this->name = this->loadingInfo.projectName;
    this->uuid = QUuid::createUuid().toString(QUuid::WithoutBraces).toUpper();

    if (!QDir().mkpath(loadingInfo.projectPath)) {
        throw ProjectException(
            "Could not create the project folder, mkdir failed on" + loadingInfo.projectPath.toStdString()
        );
    }

    QDir proposedDir(loadingInfo.projectPath);

    QDir proposedRootDir = proposedDir;
    if (!proposedRootDir.cdUp()) {
        throw ProjectException(
            "Could not create the project folder as the project folder is the root folder"
        );
    }

    this->dir = proposedDir;
    this->rootDir = proposedRootDir;


    if (!this->dir.mkdir("DCIM") ||
        !this->dir.mkdir("DFSegments") ||
        !this->dir.mkdir("DFVideos") ||
        !this->dir.mkdir("DFLowSegments") ||
        !this->dir.mkdir("DFLowVideos") ||
        !this->dir.mkdir("EVideos") ||
        !this->dir.mkdir("ELowVideos")
    ) {
        this->dir.removeRecursively();
        throw ProjectException(
            "Could not mkdir the project required folders in" + this->dir.absolutePath().toStdString()
        );
        return;
    }

    if (!this->dcimLinked) {
        this->dcim = QDir(dir.filePath("DCIM"));
    }

    this->dcim.mkdir("100GFRNT");
    this->dcim.mkdir("100GBACK");

    this->frontDir = QDir(this->dcim.filePath("100GFRNT"));
    this->backDir = QDir(this->dcim.filePath("100GBACK"));

    this->dbManager = new ProjectDB(this->dir.filePath(this->name)+".db",
                                    this->uuid, this->name, this->dcim.absolutePath(),
                                    this->dcimLinked);
}

void Project::createProjectTCopyDCIM()
{
    this->loadingProgress.stepID = COPY_DCIM_FOLDER;
    this->loadingProgress.stepNumber++;
    emit loadProjectUpdate(this->loadingProgress);

    QDir sourceFrontDir(this->loadingInfo.frontVolumePath);
    if (!sourceFrontDir.cd("DCIM") || !sourceFrontDir.cd("100GFRNT")) {
        throw ProjectException(
            "Could not find DCIM/100GFRNT folder on source front dir: " +
            sourceFrontDir.absolutePath().toStdString()
        );
    }

    QDir sourceBackDir(this->loadingInfo.backVolumePath);
    if (!sourceBackDir.cd("DCIM") || !sourceBackDir.cd("100GBACK")) {
        throw ProjectException(
            "Could not find DCIM/100GBACK folder on source front dir: " +
            sourceBackDir.absolutePath().toStdString()
        );
    }

    QFileInfoList frontFiles = sourceFrontDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
    QFileInfoList backFiles = sourceBackDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);

    this->loadingProgress.copy.fileCount = frontFiles.length() + backFiles.length();

    for (int i=0; i<frontFiles.length(); i++) {
        const QFileInfo frontFileInfo = frontFiles.at(i);

        this->loadingProgress.copy.currentFile.name = frontFileInfo.fileName();
        this->loadingProgress.copy.currentFile.bytesDone = 0;
        this->loadingProgress.copy.currentFile.bytesCount = frontFileInfo.size();
        this->loadingProgress.copy.fileNumber = i+1;
        emit loadProjectUpdate(this->loadingProgress);

        QString src = frontFileInfo.absoluteFilePath();
        QString dst = this->frontDir.filePath(frontFileInfo.fileName());

        if (QFile::exists(dst) && !QFile::remove(dst)) {
            throw ProjectException(
                "File exists in front destination and could not be removed: " + dst.toStdString(),
                "There was an error copying the source files"
            );
        }

        if (!createProjectTHCopy(src, dst)) {
            throw ProjectException(
                "Could not copy file " + src.toStdString() + " to front " + dst.toStdString(),
                "There was an error copying the source files"
            );
        }
    }

    for (int i=0; i<backFiles.length(); i++) {
        const QFileInfo backFileInfo = backFiles.at(i);

        this->loadingProgress.copy.currentFile.name = backFileInfo.fileName();
        this->loadingProgress.copy.currentFile.bytesDone = 0;
        this->loadingProgress.copy.currentFile.bytesCount = backFileInfo.size();
        this->loadingProgress.copy.fileNumber++;
        emit loadProjectUpdate(this->loadingProgress);

        QString src = backFileInfo.absoluteFilePath();
        QString dst = this->backDir.filePath(backFileInfo.fileName());

        if (QFile::exists(dst) && !QFile::remove(dst)) {
            throw ProjectException(
                "File exists in back destination and could not be removed: " + dst.toStdString(),
                "There was an error copying the source files"
            );
        }

        if (!createProjectTHCopy(src, dst)) {
            throw ProjectException(
                "Could not copy file " + src.toStdString() + " to back " + dst.toStdString(),
                "There was an error copying the source files"
            );
        }
    }
}

bool Project::createProjectTHCopy(QString src, QString dst)
{
    QFile srcFile(src);
    QFile dstFile(dst);

    if (!srcFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open source file:" << src;
        return false;
    }

    if (!dstFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "Cannot open destination file:" << dst;
        return false;
    }

    constexpr qint64 chunkSize = 4 * 1024 * 1024;
    constexpr qint64 updateInterval = 100;
    constexpr qint64 speedUpdateInterval = 1000;

    QElapsedTimer updateTimer;
    QElapsedTimer speedTimer;
    QElapsedTimer totalTimer;

    updateTimer.start();
    speedTimer.start();
    totalTimer.start();

    while (!srcFile.atEnd()) {
        QByteArray chunk = srcFile.read(chunkSize);

        if (chunk.isEmpty()) {
            if (srcFile.atEnd())
                break;

            qWarning() << "Error reading source file:" << src;
            return false;
        }

        if (dstFile.write(chunk) != chunk.size()) {
            qWarning() << "Error writing destination file:" << dst;
            return false;
        }

        this->loadingProgress.copy.currentFile.bytesDone += chunk.size();

        if (speedTimer.elapsed() >= speedUpdateInterval || this->loadingProgress.copy.currentFile.speed == 0) {
            const double speed =
                static_cast<double>(
                    this->loadingProgress.copy.currentFile.bytesDone
                    ) * 1000.0
                / totalTimer.elapsed()
                / (1024.0 * 1024.0);

            this->loadingProgress.copy.currentFile.speed = speed;

            speedTimer.restart();
        }

        if (updateTimer.elapsed() >= updateInterval) {
            emit loadProjectUpdate(this->loadingProgress);
            updateTimer.restart();
        }
    }

    const double speed =
        totalTimer.elapsed() > 0
            ? static_cast<double>(
                  this->loadingProgress.copy.currentFile.bytesDone
                  ) * 1000.0
                  / totalTimer.elapsed()
                  / (1024.0 * 1024.0)
            : 0.0;

    this->loadingProgress.copy.currentFile.speed = speed;
    emit loadProjectUpdate(this->loadingProgress);

    return true;
}

void Project::indexVideos()
{
    this->loadingProgress.stepID = INDEX_VIDEOS;
    this->loadingProgress.stepNumber++;

    QDir frontAux = this->frontDir;
    QDir backAux = this->backDir;

    frontAux.setNameFilters({"GPFR????.MP4"});
    QFileInfoList mainFrontSegments = frontAux.entryInfoList(QDir::Files);

    frontAux.setNameFilters({"GPFR????.MP4", "GF??????.MP4"});
    this->loadingProgress.index.totalSegments += frontAux.entryInfoList(QDir::Files).length();

    frontAux.setNameFilters({"GPFR????.MP4"});
    this->loadingProgress.index.totalVideos = frontAux.entryInfoList(QDir::Files).length();

    frontAux.setNameFilters({"GPBK????.MP4", "GB??????.MP4"});
    this->loadingProgress.index.totalSegments += frontAux.entryInfoList(QDir::Files).length();

    emit loadProjectUpdate(this->loadingProgress);

    for (QFileInfo& mainFrontSegment: mainFrontSegments) {
        bool isNumber = false;

        int vid = QStringView(mainFrontSegment.fileName()).mid(4,4).toInt(&isNumber);
        if (!isNumber) {
            qWarning() << "Found a main front segment with an invalid ID: " << mainFrontSegment.absoluteFilePath();
            continue;
        }

        FVideo video(vid);
        QString vidS = video.getIdString();

        // Front and back thumnails

        QString frontThmPath = frontDir.filePath("GPFR" + video.getIdString() + ".THM");
        QString backThmPath = backDir.filePath("GPBK" + video.getIdString() + ".THM");


        if (!video.setFrontThumbnail(frontThmPath)) {
            badVideos.insert(vidS, "Invalid front thumbnail");
            continue;
        }

        if (!video.setBackThumbnail(backThmPath)) {
            badVideos.insert(vidS, "Invalid back thumbnail");
            continue;
        }

        // Main segment

        FSegment mainSegment(vid, 0);

        QString frontVideoPath = frontDir.filePath("GPFR"+video.getIdString()+".MP4");
        QString frontLRVPath = frontDir.filePath("GPFR"+video.getIdString()+".LRV");

        if (!mainSegment.setFrontFiles(frontVideoPath, frontLRVPath)) {
            badVideos.insert(vidS, "main segment front files are not valid");
            continue;
        }

        QString backVideoPath = backDir.filePath("GPBK"+video.getIdString()+".MP4");
        QString backLRVPath = backDir.filePath("GPBK"+video.getIdString()+".LRV");
        QString backAudioPath = backDir.filePath("GPBK"+video.getIdString()+".WAV");

        if (!mainSegment.setBackFiles(backVideoPath, backLRVPath, backAudioPath)) {
            badVideos.insert(vidS, "main segment back files are not valid");
            continue;
        }

        video.setFormat(mainSegment.getFrontVideoFormat());
        video.addSegment(mainSegment);

        indexSegmentComplete();

        frontAux.setNameFilters({"GF??" + vidS + ".MP4"});
        QFileInfoList mainSecSegments = frontAux.entryInfoList(QDir::Files);

        bool secSegmentsOk = true;
        for (QFileInfo& mainSecSegment: mainSecSegments) {
            bool isNumber = false;
            int segId = QStringView(mainSecSegment.fileName()).mid(2,2).toInt(&isNumber);
            if (!isNumber) {
                qWarning() << "Found a secondary front segment with an invalid ID: " << mainFrontSegment.absoluteFilePath();
                badVideos.insert(vidS, "Secondary video segment invalid");
                secSegmentsOk = false;
                break;
            }

            QString segIdString = mainSecSegment.fileName().mid(2,2);

            FSegment secSegment(vid, segId);

            QString frontVideoPath = frontDir.filePath("GF"+segIdString+video.getIdString()+".MP4");
            QString frontLRVPath = frontDir.filePath("GF"+segIdString+video.getIdString()+".LRV");

            if (!secSegment.setFrontFiles(frontVideoPath, frontLRVPath)) {
                badVideos.insert(vidS, "sec segment " + segIdString + " front files are not valid");
                continue;
            }

            QString backVideoPath = backDir.filePath("GB"+segIdString+video.getIdString()+".MP4");
            QString backLRVPath = backDir.filePath("GB"+segIdString+video.getIdString()+".LRV");
            QString backAudioPath = backDir.filePath("GB"+segIdString+video.getIdString()+".WAV");

            if (!secSegment.setBackFiles(backVideoPath, backLRVPath, backAudioPath)) {
                badVideos.insert(vidS, "sec segment " + segIdString + " back files are not valid");
                continue;
            }

            if (!video.addSegment(secSegment)) {
                badVideos.insert(vidS, "Could not add sec segment " + segIdString + " to the video");
                continue;
            }

            indexSegmentComplete();
        }

        if (!secSegmentsOk) continue;

        this->videos.append(video);

        indexVideoComplete();
    }
}

void Project::indexSegmentComplete()
{
    this->loadingProgress.index.doneSegments++;
    emit loadProjectUpdate(this->loadingProgress);
}

void Project::indexVideoComplete()
{

    this->loadingProgress.index.doneVideos++;
    emit loadProjectUpdate(this->loadingProgress);
}

void Project::save()
{
    this->lastSaved = QDateTime::currentDateTime();

    if (this->dbManager == nullptr) {
        qWarning() << "dbManager is nullptr";
    } else {
        this->dbManager->updateVideos(this->videos);
    }

    if (this->localDBManager == nullptr) {
        qWarning() << "dbManager is nullptr";
    } else {
        this->localDBManager->updateRecentProjects(
            this->uuid, this->dir.absolutePath(),
            this->name, this->lastSaved
        );
    }
}
