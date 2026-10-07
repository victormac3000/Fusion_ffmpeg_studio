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
                this->createProjectSD();
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

/*

void Project::load(LoadingInfo loadingInfo)
{
    this->path = loadingInfo.projectPath;
    this->rootPath = loadingInfo.rootProjectPath;

    QString connectionName = "load_connection";

    if (QSqlDatabase::contains(connectionName)) {
        QSqlDatabase::removeDatabase(connectionName);
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName(this->path + "/project.ffs");

    if (!db.open()) {
        qWarning() << "Could not open project: Could not open the project database on "
                          + this->path + "/project.ffs : " + db.lastError().text();
        return;
    }

    QString sqlQuery = R"(
        SELECT * FROM project_info
    )";

    QSqlQuery query(db);

    if (!query.exec(sqlQuery)) {
        qWarning() << "Could not open project: select project_info Query failed with error"
                   << query.lastError().text();
        return;
    }

    if (!query.next()) {
        qWarning() << "Could not open project: Project info not found on database"
                   << this->path + "/project.ffs";
        return;
    }

    QString uuid = query.value("uuid").toString();
    QString dcim = query.value("dcim").toString();
    bool dcimLinked = query.value("dcimLinked").toBool();

    if (uuid.isEmpty()) {

    }


    this->uuid = uuid;
    this->dcim = dcim;
    this->dcimLinked = dcimLinked;

    QList<int> savedVersionNumbers = {
        query.value("version_major").toInt(),
        query.value("version_mid").toInt(),
        query.value("version_minor").toInt()
    };
    QList<int> versionNumbers = getVersionNumbers();

    if (savedVersionNumbers.at(0) < versionNumbers.at(0) ||
        savedVersionNumbers.at(1) < versionNumbers.at(1) ||
        savedVersionNumbers.at(2) < versionNumbers.at(2)) {
        QString warn("Project file has outdated version"
                    + QString::number(savedVersionNumbers.at(0)) + "."
                    + QString::number(savedVersionNumbers.at(1)) + "."
                    + QString::number(savedVersionNumbers.at(2)) + " "
                    + "compared to program version "
                    + QCoreApplication::applicationVersion()
                    + " The project will be updated to the latest version");
        qWarning() << warn;
        warnings.append(warn);
    }

    if (savedVersionNumbers.at(0) > versionNumbers.at(0) ||
        savedVersionNumbers.at(1) > versionNumbers.at(1) ||
        savedVersionNumbers.at(2) > versionNumbers.at(2)) {
        QString warn("You must update the program to at least the version"
                     + QString::number(savedVersionNumbers.at(0)) + "."
                     + QString::number(savedVersionNumbers.at(1)) + "."
                     + QString::number(savedVersionNumbers.at(2)) + " "
                     + "to open this project. The current installed program version is "
                     + QCoreApplication::applicationVersion()
                     + " The project will be updated to the latest version");
        qWarning() << warn;
        warnings.append(warn);
    }

    if (!QDir(path).exists("DFSegments") || !QDir(path).exists("DFLowSegments") ||
        !QDir(path).exists("DFVideos") || !QDir(path).exists("DFLowVideos") ||
        !QDir(path).exists("EVideos") || !QDir(path).exists("ELowVideos") ||
        (!dcimLinked && !QDir(path).exists("DCIM"))) {
        qWarning() << "Project folder invalid, required folders not found" << path;
        return;
    }

    sqlQuery = R"(
        SELECT * FROM videos
    )";

    query = QSqlQuery(db);

    if (!query.exec(sqlQuery)) {
        qWarning() << "Could not open project: select videos Query failed with error"
                   << query.lastError().text();
        return;
    }

    while (query.next()) {
        int videoId = query.value("id").toInt();
        qint64 videoDualFisheye = query.value("dualFisheye").toLongLong();


    }

    db.close();

    this->valid = true;


    if (!mainObj.value("videos").isArray()) {
        qWarning() << "Videos array not found in project file";
        return;
    }

    QJsonArray videosArray = mainObj.value("videos").toArray();

    //emit loadProjectUpdate(0, "Indexing videos");

    bool validVideo = false;
    for (const QJsonValue &videoArray: videosArray) {
        if (!videoArray.isObject()) {
            qWarning() << "Found a non object in video array parsing project file";
            continue;
        }

        QJsonObject videoObject = videoArray.toObject();
        int vid = videoObject.value("id").toInt(-1);
        if (vid<0) {
            qWarning() << "Found a video object with a non integer ID parsing project file";
            continue;
        }

        if (!videoObject.value("segments").isArray()) {
            qWarning() << "Found a video object with no segments array parsing project file";
            badVideos.append({vid, "No segments array found"});
            continue;
        }
        QJsonArray segmentsArray = videoObject.value("segments").toArray();
        if (segmentsArray.isEmpty()) {
            qWarning() << "Found a video object with no segments parsing project file";
            badVideos.append({vid, "No segments found"});
            continue;
        }

        bool dualFisheyeExists = videoObject.contains("dualFisheye");
        bool dualFisheyeLowExists = videoObject.contains("dualFisheyeLow");
        bool equirectangularExists = videoObject.contains("equirectangular");
        bool equirectangularLowExists = videoObject.contains("equirectangularLow");
        bool frontThumbnailExists = videoObject.contains("frontThumbnail");
        bool backThumbnailExists = videoObject.contains("backThumbnail");

        qint64 dualFisheye = videoObject.value("dualFisheye").toInteger(-1);
        qint64 dualFisheyeLow = videoObject.value("dualFisheyeLow").toInteger(-1);
        qint64 equirectangular = videoObject.value("equirectangular").toInteger(-1);
        qint64 equirectangularLow = videoObject.value("equirectangularLow").toInteger(-1);
        qint64 frontThumbnail = videoObject.value("frontThumbnail").toInteger(-1);
        qint64 backThumbnail = videoObject.value("backThumbnail").toInteger(-1);

        FVideo *video = new FVideo(vid);

        if (dualFisheyeExists && dualFisheye >= 0) {
            QFile* dualFisheyeFile = new QFile(path + "/DFVideos/" + QString::number(vid) + ".MP4");
            if (!dualFisheyeFile->exists()) {
                qDebug() << "Dual fisheye exists for video"
                         << vid << "but not found in fs"
                         << dualFisheyeFile->fileName();
            }
            if (dualFisheye != dualFisheyeFile->size()) {
                qDebug() << "Dual fisheye filesize does not match for video"
                         << vid << dualFisheye << dualFisheyeFile->size();
            }
            video->setDualFisheye(dualFisheyeFile);
        }

        if (dualFisheyeLowExists && dualFisheyeLow >= 0) {
            QFile* dualFisheyeLowFile = new QFile(path + "/DFLowVideos/" + QString::number(vid) + ".MP4");
            if (!dualFisheyeLowFile->exists()) {
                qDebug() << "Dual fisheye low exists for video"
                         << vid << "but not found in fs"
                         << dualFisheyeLowFile->fileName();
            }
            if (dualFisheyeLow != dualFisheyeLowFile->size()) {
                qDebug() << "Dual fisheye low filesize does not match for video"
                         << vid << dualFisheyeLow << dualFisheyeLowFile->size();
            }
            video->setDualFisheyeLow(dualFisheyeLowFile);
        }

        if (equirectangularExists && equirectangular >= 0) {
            QFile* equirectangularFile = new QFile(path + "/EVideos/" + QString::number(vid) + ".MP4");
            if (!equirectangularFile->exists()) {
                qDebug() << "Equirectangular exists for video"
                         << vid << "but not found in fs"
                         << equirectangularFile->fileName();
            }
            if (equirectangular != equirectangularFile->size()) {
                qDebug() << "Equirectangular filesize does not match for video"
                         << vid << equirectangular << equirectangularFile->size();
            }
            video->setEquirectangular(equirectangularFile);
        }

        if (equirectangularLowExists && equirectangularLow >= 0) {
            QFile* equirectangularLowFile = new QFile(path + "/ELowVideos/" + QString::number(vid) + ".MP4");
            if (!equirectangularLowFile->exists()) {
                qDebug() << "Equirectangular low exists for video"
                         << vid << "but not found in fs"
                         << equirectangularLowFile->fileName();
            }
            if (equirectangularLow != equirectangularLowFile->size()) {
                qDebug() << "Equirectangular low filesize does not match for video"
                         << vid << equirectangularLow << equirectangularLowFile->size();
            }
            video->setEquirectangularLow(equirectangularLowFile);
        }

        if (!frontThumbnailExists || !backThumbnailExists) {
            qWarning() << "Thumnails size not found for video " << vid;
            valid = false;
            badVideos.append({vid, "Thumbnails size not found in project file"});
            delete video;
            continue;
        }

        QFile* frontThumbnailFile = new QFile(dcim.absolutePath() + "/100GFRNT/GPFR" + video->getIdString() + ".THM");
        QFile* backThumbnailFile = new QFile(dcim.absolutePath() + "/100GBACK/GPBK" + video->getIdString() + ".THM");

        if (!frontThumbnailFile->exists() || !backThumbnailFile->exists()) {
            qWarning() << "Thumnails not for "
                       << vid << "but not found in fs"
                       << "FRONT (" << frontThumbnailFile->fileName()
                       << ") BACK (" << backThumbnailFile->fileName()
                       << ")";
            badVideos.append({vid, "No thumnails found in DCIM folder"});
            continue;
        }

        if (frontThumbnail != frontThumbnailFile->size() ||
            backThumbnail != backThumbnailFile->size()) {
            qDebug() << "Thumnails filesize does not match for video"
                     << vid << frontThumbnail << frontThumbnailFile->size()
                     << backThumbnail << backThumbnailFile->size();
            badVideos.append({vid, "Thumnails size does not match"});
            continue;
        }

        video->setFrontThumbnail(frontThumbnailFile);
        video->setBackThumbnail(backThumbnailFile);

        for (const QJsonValue &segmentArray: segmentsArray) {
            if (!segmentArray.isObject()) {
                qWarning() << "A non object item found in segmentArray in project file for video"
                           << vid;
                badVideos.append({vid, "A segment was not an object in project file"});
                break;
            }

            QJsonObject segmentObject = segmentArray.toObject();
            int sid = segmentObject.value("id").toInt(-1);

            if (sid < 0) {
                qWarning() << "Segment ID invalid for video" << vid;
                badVideos.append({vid, "A segment ID was invalid in project file"});
                break;
            }

            QString sidString = QString::number(sid);
            while (sidString.length() < 2) sidString.insert(0, "0");


            QString format = segmentObject.value("format").toString();

            if (format.isEmpty()) {
                qWarning() << "A segment format is empty for video"
                           << vid;
                badVideos.append({vid, "A segment format was not found in project file"});
                break;
            }

            FFormat formatGet = FFormats::getByName(format);

            if (formatGet.name.isEmpty()) {
                qWarning() << "A segment format is invalid for video"
                           << vid;
                badVideos.append({vid, "A segment format was not found in project file"});
                break;
            }

            qint64 dualFisheye = segmentObject.value("dualFisheye").toInteger(-1);
            qint64 dualFisheyeLow = segmentObject.value("dualFisheyeLow").toInteger(-1);

            FSegment* segment;
            if (sid == 0) {
                segment = new FSegment(
                    video, sid,
                    new QFile(dcim.absolutePath() + "/100GFRNT/GPFR" + video->getIdString() + ".MP4"),
                    new QFile(dcim.absolutePath() + "/100GFRNT/GPFR" + video->getIdString() + ".LRV"),
                    new QFile(dcim.absolutePath() + "/100GBACK/GPBK" + video->getIdString() + ".MP4"),
                    new QFile(dcim.absolutePath() + "/100GBACK/GPBK" + video->getIdString() + ".LRV"),
                    new QFile(dcim.absolutePath() + "/100GBACK/GPBK" + video->getIdString() + ".WAV")
                );
            } else {
                segment = new FSegment(
                    video, sid,
                    new QFile(dcim.absolutePath() + "/100GFRNT/GF" + sidString + video->getIdString() + ".MP4"),
                    new QFile(dcim.absolutePath() + "/100GFRNT/GF" + sidString + video->getIdString() + ".LRV"),
                    new QFile(dcim.absolutePath() + "/100GBACK/GB" + sidString + video->getIdString() + ".MP4"),
                    new QFile(dcim.absolutePath() + "/100GBACK/GB" + sidString + video->getIdString() + ".LRV"),
                    new QFile(dcim.absolutePath() + "/100GBACK/GB" + sidString + video->getIdString() + ".WAV")
                );
            }

            bool dualFisheyeExists = segmentObject.contains("dualFisheye");
            bool dualFisheyeLowExists = segmentObject.contains("dualFisheyeLow");
            bool frontMP4Exists = segmentObject.contains("frontMP4");
            bool frontLRVExists = segmentObject.contains("frontLRV");
            bool backMP4Exists = segmentObject.contains("backMP4");
            bool backLRVExists = segmentObject.contains("backLRV");
            bool backWAVExists = segmentObject.contains("backWAV");

            qint64 dualFisheyeSize = segmentObject.value("dualFisheye").toInteger(-1);
            qint64 dualFisheyeLowSize = segmentObject.value("dualFisheyeLow").toInteger(-1);
            qint64 frontMP4Size = frontMP4Exists ? segment->getFrontMP4()->size() : -1;
            qint64 frontLRVSize = frontLRVExists ? segment->getFrontLRV()->size() : -1;
            qint64 backMP4Size = backMP4Exists ? segment->getBackMP4()->size() : -1;
            qint64 backLRVSize = backLRVExists ? segment->getBackLRV()->size() : -1;
            qint64 backWAVSize = backWAVExists ? segment->getBackWAV()->size() : -1;

            if (frontMP4Size < 0 || frontLRVSize < 0 || backMP4Size < 0 ||
                backLRVSize < 0 || backWAVSize < 0) {
                qWarning() << "A segment" << sid
                           << "does not match size for video"
                           << vid;
                badVideos.append({vid, "One of the segment components size does not match the project file"});
                delete segment;
                break;
            }

            segment->setFormat(formatGet);

            if (dualFisheyeExists && dualFisheyeSize > 0) {
                QFile* dualFisheyeFile = new QFile(path + "/DFSegments/" + QString::number(vid) + "_" + QString::number(sid) + ".MP4");
                if (!dualFisheyeFile->exists()) {
                    qDebug() << "Dual fisheye exists for segment"
                             << sid << "of video" << vid
                             << "but not found in fs"
                             << dualFisheyeFile->fileName();
                }
                if (dualFisheye != dualFisheyeFile->size()) {
                    qDebug() << "Dual fisheye filesize does not match for segment"
                             << sid << "of video" << vid
                             << dualFisheye << dualFisheyeFile->size();
                }
                segment->setDualFisheye(dualFisheyeFile);
            }

            if (dualFisheyeLowExists && dualFisheyeLowSize > 0) {
                QFile* dualFisheyeLowFile = new QFile(path + "/DFLowSegments/" + QString::number(vid) + "_" + QString::number(sid) + ".MP4");
                if (!dualFisheyeLowFile->exists()) {
                    qDebug() << "Dual fisheye low exists for segment "
                             << sid << "of video" << vid
                             << "but not found in fs"
                             << dualFisheyeLowFile->fileName();
                }
                if (dualFisheyeLow != dualFisheyeLowFile->size()) {
                    qDebug() << "Dual fisheye low filesize does not match for segment"
                             << sid << "of video" << vid
                             << dualFisheyeLow << dualFisheyeLowFile->size();
                }
                segment->setDualFisheye(dualFisheyeLowFile);
            }

            if (!video->addSegment(segment, FILES_ONLY)) {
                qDebug() << "Segment invalid" << sid << "for video" << vid;
                badVideos.append({vid, "A segment ID was invalid in project file. View the logs for more information"});
                break;
            }

            validVideo = true;
        }

        if (validVideo) this->videos.append(video);
        progress.index.doneVideos++;
        emit loadProjectUpdate(progress);
    }

    file->close();
    this->valid = true;
}

*/

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

void Project::createProjectTGenerate()
{
    this->loadingProgress.stepID = GENERATE_PROJECT_DIRS;
    this->loadingProgress.stepNumber = 0;
    emit loadProjectUpdate(this->loadingProgress);

    this->name = this->loadingInfo.projectName;
    this->uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);

    if (!QDir().mkpath(loadingInfo.projectPath)) {
        throw ProjectException(
            "Could not create the project folder, mkdir failed on" + loadingInfo.rootProjectPath.toStdString()
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

void Project::loadProject()
{
    this->save();
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
