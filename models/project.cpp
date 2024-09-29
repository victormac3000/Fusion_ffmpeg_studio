#include "project.h"
#include "fvideo.h"
#include "loading.h"
#include "models/fformats.h"

Project::Project(QObject *parent)
    : QObject{parent}
{

}

Project::~Project()
{
    this->save();
    delete file;
}

bool Project::isValid()
{
    return valid;
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

QList<QPair<int,QString>> Project::getBadVideos()
{
    return badVideos;
}

void Project::load(LoadingInfo loadingInfo)
{
    this->path = loadingInfo.projectPath;
    this->file = new QFile(loadingInfo.projectPath + "/project.ffs");

    if (!file->exists()) {
        qWarning() << "ProjectFile QFile does not exist" << file->fileName();
        return;
    }

    if (!file->open(QFile::ReadWrite | QFile::ExistingOnly)) {
        qWarning() << "Could not open project file" << file->fileName();
        return;
    }

    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(file->readAll(), &jsonError);

    if (jsonError.error != QJsonParseError::NoError) {
        file->close();
        qWarning() << "Could not parse project file" << jsonError.errorString();
        return;
    }

    if (!jsonDoc.isObject()) {
        file->close();
        qWarning() << "The project file is not a json object";
        return;
    }

    QJsonObject mainObj = jsonDoc.object();

    if (mainObj.value("info").toObject().value("dcim").toString().isEmpty() ||
        !mainObj.value("info").toObject().contains("dcimLinked") ||
        mainObj.value("info").toObject().value("version").toString().isEmpty()) {
        file->close();
        qWarning() << "Info object: one of its children (dcim, dcimLinked or version) not found or empty";
        return;
    }

    if (!mainObj.value("videos").isArray()) {
        file->close();
        qWarning() << "Videos object is not an array";
        return;
    }

    this->dcim = mainObj.value("info").toObject().value("dcim").toString();
    this->dcimLinked = mainObj.value("info").toObject().value("dcimLinked").toBool();

    if (!this->dcimLinked) {
        this->dcim = this->path + "/DCIM";
    }

    // Check if the project version is compatible

    this->version = mainObj.value("info").toObject().value("version").toString();

    QStringList appVersionParts = QCoreApplication::applicationVersion().split(".");
    QStringList versionParts = version.split(".");

    if (appVersionParts.length() != versionParts.length()) {
        valid = false;
        qWarning() << "The application version parts length is not the same as the project version parts";
        return;
    }

    bool compatibleVersion = true;
    for (int i=0; i<versionParts.length(); i++) {
        if (appVersionParts.at(i).toInt() < versionParts.at(i).toInt()) {
            compatibleVersion = false;
        }
    }

    if (!compatibleVersion) {
        qWarning() << "The project version" << version
                   << "is not compatible with the application version"
                   << QCoreApplication::applicationVersion();
        return;
    }

    if (!QDir(path).exists("DFSegments") || !QDir(path).exists("DFLowSegments") ||
        !QDir(path).exists("DFVideos") || !QDir(path).exists("DFLowVideos") ||
        !QDir(path).exists("EVideos") || !QDir(path).exists("ELowVideos") ||
        (!dcimLinked && !QDir(path).exists("DCIM"))) {
        file->close();
        qWarning() << "Project folder invalid, required folders not found" << path;
        return;
    }

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
            emit loadProjectError("Could not read the DCIM folder");
            qWarning() << "DCIM folder invalid, not readable" << loadingInfo.dcimPath
                       << "The permissions on the folder are " << QFileInfo(loadingInfo.dcimPath).permissions();
            return;
        }
    } else {
        if (!QFileInfo(loadingInfo.dcimFrontPath).isReadable()) {
            emit loadProjectError("Could not read the front folder of the SD card");
            qWarning() << "front folder of sd card invalid, not readable" << loadingInfo.dcimFrontPath
                       << "The permissions on the front folder are " << QFileInfo(loadingInfo.dcimFrontPath).permissions();
            return;
        }
        if (!QFileInfo(loadingInfo.dcimBackPath).isReadable()) {
            emit loadProjectError("Could not read the front folder of the SD card");
            qWarning() << "front folder of sd card invalid, not readable" << loadingInfo.dcimBackPath
                       << "The permissions on the back folder are " << QFileInfo(loadingInfo.dcimBackPath).permissions();
            return;
        }
    }

    // Create project folder

    if (!QDir(loadingInfo.rootProjectPath).mkdir(loadingInfo.projectName)) {
        emit loadProjectError("Could not create the project folder");
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
        emit loadProjectError("Could not create the project subfolders");
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

    this->rootPath = loadingInfo.rootProjectPath;
    this->path = loadingInfo.projectPath;
    this->file = new QFile(path + "/project.ffs");
    this->dcim = QFileInfo(this->front.absolutePath()).absolutePath();

    if (loadingInfo.copyDCIM && !copyDCIM()) return;
    if (!indexVideos()) return;

    this->save();
    this->valid = true;
}

bool Project::copyDCIM()
{
    progress.stepID = COPY_DCIM_FOLDER;
    progress.stepNumber++;

    QDir projectDir(path);

    if (!projectDir.mkpath("DCIM/100GFRNT") ||
        !projectDir.mkpath("DCIM/100GBACK")) {
        emit loadProjectError("Could not copy the DCIM files");
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
            emit loadProjectError("Could not copy the DCIM files");
            return false;
        }
        if (!copy(src, dst)) {
            qWarning() << "Could not copy file "<< src << " to " << dst;
            emit loadProjectError("Could not copy the DCIM files");
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
            emit loadProjectError("Could not copy the DCIM files");
            return false;
        }
        if (!copy(src, dst)) {
            qWarning() << "Could not copy file "<< src << " to " << dst;
            emit loadProjectError("Could not copy the DCIM files");
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
    QList<FVideo*> videos;

    if (!front.exists()) {
        emit loadProjectError("The front folder has an invalid structure");
        qWarning() << "Front folder does not exist: " << front.absolutePath();
        return false;
    }

    if (!back.exists()) {
        emit loadProjectError("The back folder has an invalid structure");
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

        FVideo *video = new FVideo(vid);

        QFile* frontThumnailFile = new QFile(front.path() + "/GPFR" + video->getIdString() + ".THM");
        QFile* backThumnailFile = new QFile(back.path() + "/GPBK" + video->getIdString() + ".THM");

        if (!frontThumnailFile->exists() || !backThumnailFile->exists()) {
            qWarning() << "Front or back thumnails do not exist for video"
                       << vid << frontThumnailFile->fileName()
                       << backThumnailFile->fileName();
            badVideos.append({vid, "Front or back thumnails do not exist"});
            delete frontThumnailFile;
            delete backThumnailFile;
            delete video;
            continue;
        }

        video->setFrontThumbnail(frontThumnailFile);
        video->setBackThumbnail(backThumnailFile);

        if (!video->verify()) {
            badVideos.append({vid, "Invalid thumnails"});
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
            badVideos.append({vid, "Invalid main segment"});
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
                badVideos.append({vid, "Secondary video segment invalid"});
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
                badVideos.append({vid, "Secondary video segment invalid"});
                secSegmentsOk = false;
                break;
            }

            indexSegmentComplete();
        }

        if (!secSegmentsOk) continue;

        videos.append(video);
        indexVideoComplete();
    }

    this->videos = videos;

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

void Project::save()
{
    if (file == nullptr) {
        qWarning() << "Project file pointer is null";
        return;
    }

    qDebug() << "Trying to save to project file" << file->fileName();

    if (file->isOpen()) file->close();

    if (!file->open(QFile::ReadWrite | QFile::Truncate)) {
        qWarning() << "Could not open project file" << file->fileName();
        return;
    }

    QJsonDocument jsonDoc;
    QJsonObject mainObj;

    QJsonObject info;

    info.insert("dcim", dcimLinked ? dcim.absolutePath() : path + "/DCIM");
    info.insert("dcimLinked", dcimLinked);
    info.insert("version", QCoreApplication::applicationVersion());

    mainObj.insert("info", info);

    QJsonArray videosArray;

    for (FVideo *video: videos) {
        videosArray.append(video->toJson());
    }

    mainObj.insert("videos", videosArray);

    jsonDoc.setObject(mainObj);

    qint64 written = file->write(jsonDoc.toJson(QJsonDocument::Indented));
    if (written < 0) {
        file->close();
        qWarning() << "Could not save the project" << path;
        return;
    }

    file->close();

    lastSaved = QDateTime::currentDateTime();
    addToRecent();
}

void Project::addToRecent()
{
    QSettings settings;
    QFile recentProjectsFile = QFile(settings.value("appData").toString() + "/recent_projects.json");

    if (!recentProjectsFile.open(QFile::ReadWrite)) {
        qWarning() << "Could not open recent projects file";
        return;
    }

    QJsonDocument doc;
    QJsonArray docArray;
    QJsonObject obj;

    obj.insert("path", this->path);
    obj.insert("last_opened", this->lastSaved.toString(Qt::RFC2822Date));

    if (recentProjectsFile.exists()) {
        QJsonParseError parseError;
        doc = QJsonDocument::fromJson(recentProjectsFile.readAll(), &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qWarning() << "Could not parse the recent projects file";
            return;
        }

        if (!doc.isArray()) {
            qWarning() << "Could not parse the recent projects file. The main object is not an array";
            return;
        }

        docArray = doc.array();

        recentProjectsFile.close();

        if (!recentProjectsFile.open(QFile::ReadWrite | QFile::Truncate)) {
            qWarning() << "Could not truncate the recent projects file";
            return;
        }
    }


    bool projectExists = false;
    for (int i=0; i<docArray.size(); i++) {
        QJsonObject obj = docArray.at(i).toObject();

        if (obj.value("path").toString() == this->path) {
            projectExists = true;
            obj["path"] = this->path;
            obj["last_opened"] = this->lastSaved.toString(Qt::RFC2822Date);
        }
    }
    if (!projectExists) docArray.append(obj);

    doc.setArray(docArray);

    int written = recentProjectsFile.write(doc.toJson());

    if (written < 0) {
        qWarning() << "Recent projects could not be written";
    }

    recentProjectsFile.close();
}
