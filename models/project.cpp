#include "project.h"
#include "fvideo.h"
#include "loading.h"
#include "models/fformats.h"
#include "utils/settings.h"

#include <QElapsedTimer>

Project::Project(QObject *parent)
    : QObject{parent}
{

}

Project::~Project()
{
    qDeleteAll(videos);
    videos.clear();
    this->save();
}

void Project::moveToNewThread(QThread *newThread)
{
    this->moveToThread(newThread);

    for (FVideo* video : videos) {
        if (video) {
            video->moveToThread(newThread);
        }
    }
}

bool Project::isValid()
{
    return valid;
}

QStringList Project::getErrors()
{
    return errors;
}

QStringList Project::getWarnings()
{
    return warnings;
}

QDir Project::getDcim()
{
    return dcim;
}

void Project::setDcim(QString newDcim)
{
    dcim = newDcim;
}

QString Project::getVersion()
{
    return version;
}

void Project::setVersion(QString newVersion)
{
    version = newVersion;
}

QString Project::getPath()
{
    return path;
}

void Project::setVideos(QList<FVideo*> videos)
{
    this->videos = videos;
}

QList<FVideo*> Project::getVideos()
{
    return videos;
}

QList<QPair<QString,QString>> Project::getBadVideos()
{
    return badVideos;
}

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
    /*



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

    */
}

void Project::create(LoadingInfo loadingInfo)
{
    progress.stepCount = loadingInfo.copyDCIM ? 4 : 3;
    progress.stepID = CHECK_SOURCE_FOLDERS;
    progress.stepNumber = 1;
    emit loadProjectUpdate(progress);

    // Check that the DCIM folder is readable

    if (loadingInfo.type == CREATE_PROJECT_FOLDER) {
        if (!QFileInfo(loadingInfo.dcimPath).isReadable() ||
            !QFileInfo(loadingInfo.dcimPath + "/100GFRNT").isReadable() ||
            !QFileInfo(loadingInfo.dcimPath + "/100GBACK").isReadable()) {
            qWarning() << "DCIM folder invalid, not readable" << loadingInfo.dcimPath
                       << "The permissions on the folder are " << QFileInfo(loadingInfo.dcimPath).permissions();
            return;
        }
    } else {
        if (!QFileInfo(loadingInfo.dcimFrontPath).isReadable()) {
            qWarning() << "front folder of sd card invalid, not readable" << loadingInfo.dcimFrontPath
                       << "The permissions on the front folder are " << QFileInfo(loadingInfo.dcimFrontPath).permissions();
            return;
        }
        if (!QFileInfo(loadingInfo.dcimBackPath).isReadable()) {
            qWarning() << "back folder of sd card invalid, not readable" << loadingInfo.dcimBackPath
                       << "The permissions on the back folder are " << QFileInfo(loadingInfo.dcimBackPath).permissions();
            return;
        }
    }

    // Create project folder

    if (!QDir(loadingInfo.rootProjectPath).mkdir(loadingInfo.projectName)) {
        qWarning() << "Could not create the project folder, mkdir failed on" << loadingInfo.rootProjectPath;
        return;
    }

    progress.stepID = CREATE_PROJECT_DIRS;
    progress.stepNumber++;
    emit loadProjectUpdate(progress);

    QDir projectFolder(loadingInfo.projectPath);

    if ((!projectFolder.mkdir("DFSegments")) ||
        (!projectFolder.mkdir("DFVideos")) ||
        (!projectFolder.mkdir("DFLowSegments")) ||
        (!projectFolder.mkdir("DFLowVideos")) ||
        (!projectFolder.mkdir("EVideos")) ||
        (!projectFolder.mkdir("ELowVideos"))) {
        qWarning() << "Could not mkdir the project required folders in" << projectFolder.absolutePath();
        projectFolder.removeRecursively();
        return;
    }

    if (loadingInfo.type == CREATE_PROJECT_FOLDER) {
        this->front = QDir(loadingInfo.dcimPath + "/100GFRNT");
        this->back = QDir(loadingInfo.dcimPath + "/100GBACK");
    } else {
        this->front = QDir(loadingInfo.dcimFrontPath);
        this->back = QDir(loadingInfo.dcimBackPath);
    }

    this->uuid = QUuid::createUuid().toString();
    this->rootPath = loadingInfo.rootProjectPath;
    this->path = loadingInfo.projectPath;
    this->dcim = QFileInfo(this->front.absolutePath()).absolutePath();

    if (!setupDatabase()) return;
    if (loadingInfo.copyDCIM && !copyDCIM()) return;
    if (!indexVideos()) return;
    if (!this->save()) return;

    this->valid = true;
}

bool Project::setupDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "setup_database_project");
    db.setDatabaseName(this->path + "/project.ffs");

    if (!db.open()) {
        qWarning() << "Could not create project database on " + this->path + "/project.ffs";
        return false;
    }

    QString sqlQuery = R"(
        CREATE TABLE IF NOT EXISTS project_info (
            uuid TEXT PRIMARY KEY,
            dcim TEXT NOT NULL,
            dcimLinked INTEGER NOT NULL,
            version_major INTEGER NOT NULL,
            version_mid INTEGER NOT NULL,
            version_minor INTEGER NOT NULL
        );
    )";

    QSqlQuery query(db);

    if (!query.exec(sqlQuery)) {
        qWarning() << "Could not create project database initial structure table project_info" << query.lastError().text();
        return false;
    }

    sqlQuery = R"(
        CREATE TABLE IF NOT EXISTS videos (
            id INTEGER PRIMARY KEY,
            dualFisheye INTEGER,
            dualFisheyeLow INTEGER,
            equirectangular INTEGER,
            equirectangularLow INTEGER,
            frontThumbnail INTEGER NOT NULL,
            backThumbnail INTEGER NOT NULL
        );
    )";

    if (!query.exec(sqlQuery)) {
        qWarning() << "Could not create project database initial structure table videos" << query.lastError().text();
        return false;
    }

    sqlQuery = R"(
        CREATE TABLE IF NOT EXISTS segments (
            id INTEGER PRIMARY KEY,
            video INTEGER NOT NULL,
            dualFisheye INTEGER,
            dualFisheyeLow INTEGER,
            frontMP4 INTEGER NOT NULL,
            frontLRV INTEGER NOT NULL,
            backMP4 INTEGER NOT NULL,
            backLRV INTEGER NOT NULL,
            backWAV INTEGER NOT NULL,
            format TEXT NOT NULL,
            FOREIGN KEY(video) REFERENCES videos(id) ON UPDATE CASCADE ON DELETE CASCADE
        );
    )";

    if (!query.exec(sqlQuery)) {
        qWarning() << "Could not create project database initial structure table videos" << query.lastError().text();
        return false;
    }

    db.close();
    return true;
}

bool Project::copyDCIM()
{
    progress.stepID = COPY_DCIM_FOLDER;
    progress.stepNumber++;

    QDir projectDir(path);

    if (!projectDir.mkpath("DCIM/100GFRNT") ||
        !projectDir.mkpath("DCIM/100GBACK")) {
        qWarning() << "Could not create DCIM paths in the project folder";
        return false;
    }

    QFileInfoList frontFiles = front.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
    QFileInfoList backFiles = back.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
    progress.copy.fileCount = frontFiles.length() + backFiles.length();

    int i=0;
    for (const QFileInfo &frontFileInfo: frontFiles) {
        progress.copy.currentFile.name = frontFileInfo.fileName();
        progress.copy.currentFile.bytesCount = frontFileInfo.size();
        progress.copy.fileNumber = i+1;
        emit loadProjectUpdate(progress);
        QString src = frontFileInfo.absoluteFilePath();
        QString dst = projectDir.absolutePath() + "/DCIM/100GFRNT/" + frontFileInfo.fileName();
        if (QFile::exists(dst) && !QFile::remove(dst)) {
            qWarning() << "File exists in destination and could not be removed: "<< dst;
            return false;
        }
        if (!copy(src, dst)) {
            qWarning() << "Could not copy file "<< src << " to " << dst;
            return false;
        }
        i++;
    }

    i=progress.copy.fileNumber;
    for (const QFileInfo &backFileInfo: backFiles) {
        progress.copy.currentFile.name = backFileInfo.fileName();
        progress.copy.currentFile.bytesCount = backFileInfo.size();
        progress.copy.fileNumber = i+1;
        emit loadProjectUpdate(progress);
        QString src = backFileInfo.absoluteFilePath();
        QString dst = projectDir.absolutePath() + "/DCIM/100GBACK/" + backFileInfo.fileName();
        if (QFile::exists(dst) && !QFile::remove(dst)) {
            qWarning() << "File exists in destination and could not be removed: "<< dst;
            return false;
        }
        if (!copy(src, dst)) {
            qWarning() << "Could not copy file "<< src << " to " << dst;
            return false;
        }
        i++;
    }

    this->front = QDir(projectDir.absolutePath() + "/DCIM/100GFRNT");
    this->back = QDir(projectDir.absolutePath() + "/DCIM/100GBACK");
    this->dcimLinked = false;

    return true;
}

bool Project::indexVideos()
{
    if (!front.exists()) {
        qWarning() << "Front folder does not exist: " << front.absolutePath();
        return false;
    }

    if (!back.exists()) {
        qWarning() << "Back folder does not exist: " << back.absolutePath();
        return false;
    }

    front.setNameFilters({"GPFR????.MP4"});
    QFileInfoList mainFrontSegments = front.entryInfoList(QDir::Files);

    // Count total segments in total
    progress.index.totalSegments = 0;
    QDir frontAux(front.absolutePath());

    frontAux.setNameFilters({"GPFR????.MP4", "GF??????.MP4"});
    progress.index.totalSegments += frontAux.entryInfoList(QDir::Files).length();

    frontAux.setNameFilters({"GPFR????.MP4"});
    progress.index.totalVideos = frontAux.entryInfoList(QDir::Files).length();

    frontAux.setNameFilters({"GPBK????.MP4", "GB??????.MP4"});
    progress.index.totalSegments += frontAux.entryInfoList(QDir::Files).length();

    progress.index.doneSegments = 0;
    progress.index.doneVideos = 0;

    progress.stepID = INDEX_VIDEOS;
    progress.stepNumber++;
    emit loadProjectUpdate(progress);

    for (const QFileInfo &mainFrontSegment: mainFrontSegments) {
        bool isNumber = false;

        int vid = QStringView(mainFrontSegment.fileName()).mid(4,4).toInt(&isNumber);
        if (!isNumber) {
            qWarning() << "Found a main front segment with an invalid ID: " << mainFrontSegment.absoluteFilePath();
            continue;
        }

        FVideo *video = new FVideo(this, vid);

        if (!video->setFrontThumbnail(front.path() + "/GPFR" + video->getIdString() + ".THM")) {
            badVideos.append({video->getIdString(), "Invalid front thumbnail"});
            continue;
        }

        if (!video->setBackThumbnail(back.path() + "/GPBK" + video->getIdString() + ".THM")) {
            badVideos.append({video->getIdString(), "Invalid back thumbnail"});
            continue;
        }

        // Add main segment
        FSegment *mainSegment = new FSegment(
            video, 0,
            new QFile(front.path() + "/GPFR" + video->getIdString() + ".MP4"),
            new QFile(front.path() + "/GPFR" + video->getIdString() + ".LRV"),
            new QFile(back.path() + "/GPBK" + video->getIdString() + ".MP4"),
            new QFile(back.path() + "/GPBK" + video->getIdString() + ".LRV"),
            new QFile(back.path() + "/GPBK" + video->getIdString() + ".WAV")
        );
        if (!video->addSegment(mainSegment)) {
            badVideos.append({video->getIdString(), "Invalid main segment"});
            continue;
        }
        indexSegmentComplete();

        front.setNameFilters({"GF??" + video->getIdString() + ".MP4"});
        QFileInfoList mainSecSegments = front.entryInfoList(QDir::Files);

        bool secSegmentsOk = true;
        for (const QFileInfo &mainSecSegment: mainSecSegments) {
            bool isNumber = false;
            int segId = QStringView(mainSecSegment.fileName()).mid(2,2).toInt(&isNumber);
            if (!isNumber) {
                qWarning() << "Found a secondary front segment with an invalid ID: " << mainFrontSegment.absoluteFilePath();
                badVideos.append({video->getIdString(), "Secondary video segment invalid"});
                secSegmentsOk = false;
                break;
            }

            QString segIdString = mainSecSegment.fileName().mid(2,2);

            FSegment *secSegment = new FSegment(
                video, segId,
                new QFile(front.path() + "/GF" + segIdString + video->getIdString() + ".MP4"),
                new QFile(front.path() + "/GF" + segIdString + video->getIdString() + ".LRV"),
                new QFile(back.path() + "/GB" + segIdString + video->getIdString() + ".MP4"),
                new QFile(back.path() + "/GB" + segIdString + video->getIdString() + ".LRV"),
                new QFile(back.path() + "/GB" + segIdString + video->getIdString() + ".WAV")
            );

            if (!video->addSegment(secSegment)) {
                qWarning() << "Found an invalid secondary segment for video " << vid;
                badVideos.append({video->getIdString(), "Secondary video segment invalid"});
                secSegmentsOk = false;
                break;
            }

            indexSegmentComplete();
        }

        if (!secSegmentsOk) continue;

        videos.append(video);
        indexVideoComplete();
    }

    return true;
}

void Project::indexSegmentComplete()
{
    progress.index.doneSegments++;
    emit loadProjectUpdate(progress);
}

void Project::indexVideoComplete()
{
    progress.index.doneVideos++;
    emit loadProjectUpdate(progress);
}

bool Project::copy(QString src, QString dst)
{
    QFile srcFile(src);
    QFile dstFile(dst);

    if (!srcFile.open(QFile::ReadOnly) || !dstFile.open(QFile::ReadWrite | QFile::Truncate)) {
        qWarning() << "Cannot open src file (" << src << ") or dst file (" << dst << ")";
        srcFile.close();
        dstFile.close();
        return false;
    }

    const int chunkSize = 1024;
    qint64 chunks = 0;

    QElapsedTimer tmr;
    tmr.start();
    qint64 bytesStamp = 0;
    bool firstRun = true;

    while (!srcFile.atEnd()) {
        QByteArray chunk = srcFile.read(chunkSize);
        if (dstFile.write(chunk) == -1) {
            qWarning() << "Error writing to dst file (" << dst << ")";
            srcFile.close();
            dstFile.close();
            return false;
        }
        chunks++;
        progress.copy.currentFile.bytesDone = chunks*chunkSize;
        if (firstRun && tmr.elapsed() >= 5) {
            double speed = (double) (progress.copy.currentFile.bytesDone - bytesStamp) / (double) (1024*1024);
            speed *= tmr.elapsed();
            progress.copy.currentFile.speed = speed;
            firstRun = false;
            emit loadProjectUpdate(progress);
        }
        if (tmr.elapsed() >= 100) {
            double speed = (double) (progress.copy.currentFile.bytesDone - bytesStamp) / (double) (1024*1024);
            speed *= tmr.elapsed();
            progress.copy.currentFile.speed = speed;
            emit loadProjectUpdate(progress);
            bytesStamp = progress.copy.currentFile.bytesDone;
            tmr.restart();
        }
    }

    progress.copy.currentFile.bytesDone = progress.copy.currentFile.bytesCount;
    emit loadProjectUpdate(progress);

    srcFile.close();
    dstFile.close();

    return true;
}

QList<int> Project::getVersionNumbers()
{
    QString version = QCoreApplication::applicationVersion();
    QStringList versionList = version.split(".");
    return {
        (versionList.length()>0) ? versionList.at(0).toInt() : 0,
        (versionList.length()>1) ? versionList.at(1).toInt() : 0,
        (versionList.length()>2) ? versionList.at(2).toInt() : 0
    };
}

bool Project::save()
{
    QString connectionName = "save_connection";

    if (QSqlDatabase::contains(connectionName)) {
        QSqlDatabase::removeDatabase(connectionName);
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName(this->path + "/project.ffs");

    if (!db.open()) {
        qWarning() << "Could not save project: Could not open the project database on "
                          + this->path + "/project.ffs : " + db.lastError().text();
        return false;
    }

    QString sqlQuery = R"(
        INSERT OR REPLACE INTO project_info
            (uuid, dcim, dcimLinked, version_major, version_mid, version_minor)
        VALUES
            (:uuid, :dcim, :dcimLinked, :version_major, :version_mid, :version_minor);
    )";

    QSqlQuery query(db);

    if (!query.prepare(sqlQuery)) {
        qWarning() << "Could not save project: Query preparation failed Insert/Update project info" << query.lastError().text();
        return false;
    }

    QList<int> versionNumbers = getVersionNumbers();

    query.bindValue(":uuid", this->uuid);
    query.bindValue(":dcim", this->dcim.absolutePath());
    query.bindValue(":dcimLinked", this->dcimLinked);
    query.bindValue(":version_major", versionNumbers.at(0));
    query.bindValue(":version_mid", versionNumbers.at(1));
    query.bindValue(":version_minor", versionNumbers.at(2));

    if (!query.exec()) {
        qWarning() << "Could not save project: Query failed Insert/Update project info" << query.lastError().text();
        return false;
    }

    for (FVideo *video: videos) {
        if (!db.transaction()) {
            qWarning() << "Could not save project: Could not open database transaction for video "
                              + video->getIdString() + " : " + query.lastError().text();
            return false;
        }

        sqlQuery = R"(
            INSERT OR REPLACE INTO videos
                (id, dualFisheye, dualFisheyeLow, equirectangular, equirectangularLow, frontThumbnail, backThumbnail)
            VALUES
                (:id, :dualFisheye, :dualFisheyeLow, :equirectangular, :equirectangularLow, :frontThumbnail, :backThumbnail);
        )";

        query = QSqlQuery(db);
        if (!query.prepare(sqlQuery)) {
            qWarning() << "Could not save project: Query preparation failed Insert/Update video "
                        + video->getIdString() + " : " + query.lastError().text();
            return false;
        }

        QFile* dualFisheye = video->getDualFisheye();
        QFile* dualFisheyeLow = video->getDualFisheyeLow();
        QFile* equirectangular = video->getEquirectangular();
        QFile* equirectangularLow = video->getEquirectangularLow();
        QFile* frontThumbnail = video->getFrontThumbnail();
        QFile* backThumbnail = video->getBackThumbnail();

        query.bindValue(":id", video->getId());
        query.bindValue(":dualFisheye", (dualFisheye == nullptr) ? -1 : dualFisheye->size());
        query.bindValue(":dualFisheyeLow", (dualFisheyeLow == nullptr) ? -1 : dualFisheyeLow->size());
        query.bindValue(":equirectangular", (equirectangular == nullptr) ? -1 : equirectangular->size());
        query.bindValue(":equirectangularLow", (equirectangularLow == nullptr) ? -1 : equirectangularLow->size());
        query.bindValue(":frontThumbnail", (frontThumbnail == nullptr) ? -1 : frontThumbnail->size());
        query.bindValue(":backThumbnail", (backThumbnail == nullptr) ? -1 : backThumbnail->size());

        if (!query.exec()) {
            qWarning() << "Could not save project: Query failed Insert/Update video "
                              + video->getIdString() + " : " + query.lastError().text();
            return false;
        }


        QList<FSegment*> segments = video->getSegments();
        for (FSegment *segment: segments) {
            sqlQuery = R"(
                INSERT OR REPLACE INTO segments
                    (id, video, dualFisheye, dualFisheyeLow, frontMP4, frontLRV, backMP4, backLRV, backWAV, format)
                VALUES
                    (:id, :video, :dualFisheye, :dualFisheyeLow, :frontMP4, :frontLRV, :backMP4, :backLRV, :backWAV, :format);
            )";

            query = QSqlQuery(db);
            if (!query.prepare(sqlQuery)) {
                qWarning() << "Could not save project: Query preparation failed Insert/Update segment "
                                  + segment->getIdString() + " from video " + video->getIdString() + " : " + query.lastError().text();
                return false;
            }

            QFile* dualFisheye = segment->getDualFisheye();
            QFile* dualFisheyeLow = segment->getDualFisheyeLow();
            QFile* frontMP4 = segment->getFrontMP4();
            QFile* frontLRV = segment->getFrontLRV();
            QFile* backMP4 = segment->getBackMP4();
            QFile* backLRV = segment->getBackLRV();
            QFile* backWAV = segment->getBackWAV();
            QString format = segment->getFormat().name;

            query.bindValue(":id", segment->getId());
            query.bindValue(":video", video->getId());
            query.bindValue(":dualFisheye", (dualFisheye == nullptr) ? -1 : dualFisheye->size());
            query.bindValue(":dualFisheyeLow", (dualFisheyeLow == nullptr) ? -1 : dualFisheyeLow->size());
            query.bindValue(":frontMP4", (frontMP4 == nullptr) ? -1 : frontMP4->size());
            query.bindValue(":frontLRV", (frontLRV == nullptr) ? -1 : frontLRV->size());
            query.bindValue(":backMP4", (backMP4 == nullptr) ? -1 : backMP4->size());
            query.bindValue(":backLRV", (backLRV == nullptr) ? -1 : backLRV->size());
            query.bindValue(":backWAV", (backWAV == nullptr) ? -1 : backWAV->size());
            query.bindValue(":format", format);

            if (!query.exec()) {
                qWarning() << "Could not save project: Query failed Insert/Update segment "
                                  + segment->getIdString() + " from video " + video->getIdString() + " : " + query.lastError().text();
                return false;
            }
        }

        if (!db.commit()) {
            qWarning() << "Could not save project: Could not save database transaction for video "
                              + video->getIdString() + " : " + query.lastError().text();
            return false;
        }
    }

    db.close();

    this->lastSaved = QDateTime::currentDateTime();
    addToRecent();
    return true;
}

void Project::addToRecent()
{
    QString connectionName = "add_to_recent_projects_connection";

    if (QSqlDatabase::contains(connectionName)) {
        QSqlDatabase::removeDatabase(connectionName);
    }

    QSqlDatabase localDb = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    localDb.setDatabaseName(Settings::getAppDataPath() + "/local.db");

    if (!localDb.open()) {
        qWarning() << "Could not add project to recent projects. Could not connect to database: " + localDb.lastError().text();
        return;
    }

    QString sqlQuery = R"(
        INSERT OR REPLACE INTO recent_projects
            (uuid, path, name, saved_on)
        VALUES
            (:uuid, :path, :name, :saved_on);
    )";

    QSqlQuery query(localDb);

    if (!query.prepare(sqlQuery)) {
        qWarning() << "Could not add project to recent projects. Insert/Update query failed to prepare" << query.lastError().text();
        return;
    }

    query.bindValue(":uuid", this->uuid);
    query.bindValue(":path", this->path);
    query.bindValue(":name", QFileInfo(this->path).fileName());
    query.bindValue(":saved_on", this->lastSaved.toMSecsSinceEpoch());

    if (!query.exec()) {
        qWarning() << "Could not add project to recent projects. Insert/Update query failed with error" << query.lastError().text();
        return;
    }

    localDb.close();
}
