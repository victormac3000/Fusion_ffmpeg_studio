#include "project.h"
#include "fvideo.h"
#include "loading.h"

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

void Project::load(QString projectPath)
{
    this->path = projectPath;
    this->file = new QFile(projectPath + "/project.ffs");

    if (!file->exists()) {
        qWarning() << "ProjectFile QFile does not exist" << file->fileName();
        return;
    }

    if (!file->open(QFile::ReadWrite | QFile::ExistingOnly)) {
        qWarning() << "Could not open project file" << file->fileName();
        return;
    }

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(file->readAll(), &error);

    if (error.error != QJsonParseError::NoError) {
        file->close();
        qWarning() << "Could not parse project file" << error.errorString();
        return;
    }

    if (!jsonDoc.isObject()) {
        file->close();
        qWarning() << "The project file is not a json object";
        return;
    }

    QJsonObject mainObj = jsonDoc.object();

    if (mainObj.value("info").toObject().value("name").toString().isEmpty()
        || mainObj.value("info").toObject().value("dcim").toString().isEmpty()
        || mainObj.value("info").toObject().value("version").toString().isEmpty()) {
        file->close();
        qWarning() << "Info object: one of its children (name, info or version) not found or empty";
        return;
    }

    if (!mainObj.value("videos").isArray()) {
        file->close();
        qWarning() << "Videos object is not an array";
        return;
    }

    this->name = mainObj.value("info").toObject().value("name").toString();
    this->dcim = mainObj.value("info").toObject().value("dcim").toString();


    // Check if the project version is compatible

    this->version = mainObj.value("info").toObject().value("version").toString();

    QStringList appVersionParts = QCoreApplication::applicationVersion().split(".");
    QStringList versionParts = version.split(".");

    if (appVersionParts.length() != versionParts.length()) {
        qWarning() << "The application version parts length is not the same as the project version parts";
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
    }

    if (!QDir(path).exists("DFSegments") || !QDir(path).exists("DFLowSegments") ||
        !QDir(path).exists("DFVideos") || !QDir(path).exists("DFLowVideos") ||
        !QDir(path).exists("EVideos") || !QDir(path).exists("ELowVideos")) {
        file->close();
        qWarning() << "Project folder invalid, required folders not found" << path;
        return;
    }

    QJsonArray videosArray = mainObj.value("videos").toArray();

    //emit loadProjectUpdate(0, "Indexing videos");

    int totalVideos = videosArray.size();
    int doneVideos = 0;

    for (const QJsonValue &videoArray: videosArray) {
        bool valid = true;
        if (!videoArray.isObject()) continue;
        QJsonObject videoObject = videoArray.toObject();
        int vid = videoObject.value("id").toInt(-1);
        if (vid<0) continue;
        bool dualFisheye = videoObject.value("dualFisheye").toBool();
        bool dualFisheyeLow = videoObject.value("dualFisheyeLow").toBool();
        bool equirectangular = videoObject.value("equirectangular").toBool();
        bool equirectangularLow = videoObject.value("equirectangularLow").toBool();
        if (!videoObject.value("segments").isArray()) continue;
        QJsonArray segmentsArray = videoObject.value("segments").toArray();
        if (segmentsArray.isEmpty()) continue;
        FVideo *video = new FVideo(vid);
        if (dualFisheye) {
            QString dualFisheyePath = path + "/DFVideos/" + QString::number(vid) + ".MP4";
            if (QFile::exists(dualFisheyePath)) {
                video->setDualFisheye(new QFile(dualFisheyePath));
            } else {
                qDebug() << "Dual fisheye exists for video" << vid << "but not found in fs" << dualFisheyePath;
            }
        }
        if (dualFisheyeLow) {
            QString dualFisheyeLowPath = path + "/DFLowVideos/" + QString::number(vid) + ".MP4";
            if (QFile::exists(dualFisheyeLowPath)) {
                video->setDualFisheyeLow(new QFile(dualFisheyeLowPath));
            } else {
                qDebug() << "Dual fisheye low exists for video" << vid << "but not found in fs" << dualFisheyeLowPath;
            }
        }
        if (equirectangular) {
            QString equirectangularPath = path + "/EVideos/" + QString::number(vid) + ".MP4";
            if (QFile::exists(equirectangularPath)) {
                video->setEquirectangular(new QFile(equirectangularPath));
            } else {
                qDebug() << "Equirectangular exists for video" << vid << "but not found in fs" << equirectangularPath;
            }
        }
        if (equirectangularLow) {
            QString equirectangularLowPath = path + "/ELowVideos/" + QString::number(vid) + ".MP4";
            if (QFile::exists(equirectangularLowPath)) {
                video->setEquirectangularLow(new QFile(equirectangularLowPath));
            } else {
                qDebug() << "Equirectangular preview exists for video" << vid << "but not found in fs" << equirectangularLowPath;
            }
        }
        video->setFrontThumbnail(new QFile(dcim.absolutePath() + "/100GFRNT/GPFR" + video->getIdString() + ".THM"));
        video->setBackThumbnail(new QFile(dcim.absolutePath() + "/100GBACK/GPBK" + video->getIdString() + ".THM"));
        for (const QJsonValue &segmentArray: segmentsArray) {
            if (!segmentArray.isObject()) continue;
            QJsonObject segmentObject = segmentArray.toObject();
            int sid = segmentObject.value("id").toInt(-1);
            QString sidString = QString::number(sid);
            while (sidString.length() < 2) sidString.insert(0, "0");
            bool dualFisheye = segmentObject.value("dualFisheye").toBool();
            bool dualFisheyeLow = segmentObject.value("dualFisheyeLow").toBool();
            FSegment *segment;
            if (sid == 0) {
                segment = new FSegment(
                    video, sid,
                    new QFile(dcim.absolutePath() + "/100GFRNT/GPFR" + video->getIdString() + ".MP4"),
                    new QFile(dcim.absolutePath() + "/100GFRNT/GPFR" + video->getIdString() + ".LRV"),
                    new QFile(dcim.absolutePath() + "/100GFRNT/GPFR" + video->getIdString() + ".THM"),
                    new QFile(dcim.absolutePath() + "/100GBACK/GPBK" + video->getIdString() + ".MP4"),
                    new QFile(dcim.absolutePath() + "/100GBACK/GPBK" + video->getIdString() + ".LRV"),
                    new QFile(dcim.absolutePath() + "/100GBACK/GPBK" + video->getIdString() + ".THM"),
                    new QFile(dcim.absolutePath() + "/100GBACK/GPBK" + video->getIdString() + ".WAV")
                    );
            } else {
                segment = new FSegment(
                    video, sid,
                    new QFile(dcim.absolutePath() + "/100GFRNT/GF" + sidString + video->getIdString() + ".MP4"),
                    new QFile(dcim.absolutePath() + "/100GFRNT/GF" + sidString + video->getIdString() + ".LRV"),
                    new QFile(dcim.absolutePath() + "/100GFRNT/GPFR" + video->getIdString() + ".THM"),
                    new QFile(dcim.absolutePath() + "/100GBACK/GB" + sidString + video->getIdString() + ".MP4"),
                    new QFile(dcim.absolutePath() + "/100GBACK/GB" + sidString + video->getIdString() + ".LRV"),
                    new QFile(dcim.absolutePath() + "/100GBACK/GPBK" + video->getIdString() + ".THM"),
                    new QFile(dcim.absolutePath() + "/100GBACK/GB" + sidString + video->getIdString() + ".WAV")
                    );
            }
            if (dualFisheye) {
                QString dualFisheyePath = path + "/DFSegments/" + QString::number(vid) + "_" + QString::number(sid) + ".MP4";
                if (QFile::exists(dualFisheyePath)) {
                    segment->setDualFisheye(new QFile(dualFisheyePath));
                } else {
                    qDebug() << "Dual fisheye exists for seg" << sid << "of video" << vid << "but not found in fs" << dualFisheyePath;
                }
            }
            if (dualFisheyeLow) {
                QString dualFisheyeLowPath = path + "/DFLowSegments/" + QString::number(vid) + "_" + QString::number(sid) + ".MP4";
                if (QFile::exists(dualFisheyeLowPath)) {
                    segment->setDualFisheyeLow(new QFile(dualFisheyeLowPath));
                } else {
                    qDebug() << "Dual fisheye exists for seg" << sid << "of video" << vid << "but not found in fs" << dualFisheyeLowPath;
                }
            }
            if (!video->addSegment(segment, false)) {
                qDebug() << "Segment invalid" << sid << "for video" << vid;
                valid = false;
                break;
            }
        }
        if (valid) this->videos.append(video);
        doneVideos++;
        emit indexVideoComplete();
    }

    file->close();
    this->valid = true;
}

void Project::create(LoadingInfo loadingInfo)
{
    progress.stepCount = loadingInfo.copyDCIM ? 3 : 4;
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

    QDir projectFolder(loadingInfo.rootProjectPath + "/" + loadingInfo.projectName);

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
        this->front = loadingInfo.dcimFrontPath;
        this->back = loadingInfo.dcimBackPath;
    }

    this->rootPath = loadingInfo.rootProjectPath;
    this->path = loadingInfo.projectPath;
    this->name = loadingInfo.projectName;
    this->file = new QFile(path + "/project.ffs");

    if (loadingInfo.copyDCIM && !copyDCIM()) return;
    if (!indexVideos()) return;

    this->save();
    this->valid = true;
}

void Project::save()
{
    qDebug() << "Trying to save to project file" << file->fileName();

    if (file == nullptr) {
        qWarning() << "Project file pointer is null";
        return;
    }

    if (file->isOpen()) file->close();

    if (!file->open(QFile::ReadWrite)) {
        qWarning() << "Could not open project file" << file->fileName();
        return;
    }

    QJsonDocument jsonDoc;
    QJsonObject mainObj;

    QJsonObject info;

    info.insert("name", name);
    info.insert("dcim", dcim.absolutePath());
    info.insert("version", QCoreApplication::applicationVersion());

    mainObj.insert("info", info);

    QJsonArray videosArray;

    for (FVideo *video: videos) {
        QJsonObject videoObject;
        videoObject.insert("id", video->getId());
        videoObject.insert("dualFisheye", video->isDualFisheyeValid());
        videoObject.insert("dualFisheyeLow", video->isDualFisheyeLowValid());
        videoObject.insert("equirectangular", video->isEquirectangularValid());
        videoObject.insert("equirectangularLow", video->isEquirectangularLowValid());
        QJsonArray segmentsArray;
        QList<FSegment*> segments = video->getSegments();
        for (FSegment* segment: segments) {
            QJsonObject segmentObject;
            segmentObject.insert("id", segment->getId());
            segmentObject.insert("dualFisheye", segment->isDualFisheyeValid());
            segmentObject.insert("dualFisheyeLow", segment->isDualFisheyeLowValid());
            segmentsArray.append(segmentObject);
        }
        videoObject.insert("segments", segmentsArray);
        videosArray.append(videoObject);
    }

    mainObj.insert("videos", videosArray);
    jsonDoc.setObject(mainObj);

    int written = file->write(jsonDoc.toJson(QJsonDocument::Indented));
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

    bool fileExists = recentProjectsFile.exists();

    if (!recentProjectsFile.open(QFile::ReadWrite)) {
        qWarning() << "Could not open recent projects file";
        return;
    }

    QJsonDocument doc;
    QJsonArray docArray;
    QJsonObject obj;

    obj.insert("name", this->name);
    obj.insert("path", this->path);
    obj.insert("last_opened", this->lastSaved.toString(Qt::RFC2822Date));

    if (fileExists) {
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
            obj["name"] = this->name;
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

bool Project::copyDCIM()
{
    progress.stepID = COPY_DCIM_FOLDER;
    progress.stepNumber++;
    emit loadProjectUpdate(progress);

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
        progress.copy.progress = i/progress.copy.fileCount;
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

    i=0;
    for (const QFileInfo &backFileInfo: backFiles) {
        progress.copy.currentFile.name = backFileInfo.fileName();
        progress.copy.currentFile.bytesCount = backFileInfo.size();
        progress.copy.fileNumber = i+1;
        progress.copy.progress = i/progress.copy.fileCount;
        QString src = backFileInfo.absoluteFilePath();
        QString dst = projectDir.absolutePath() + "/DCIM/100GBACK/" + backFileInfo.fileName();
        if (QFile::exists(dst) && !QFile::remove(dst)) {
            qWarning() << "File exists in destination and could not be removed: "<< dst;
            emit loadProjectError("Could not copy the DCIM files");
            return false;
        }
        qDebug() << src << dst;
        if (!QFile(src).copy(dst)) {
            qWarning() << "Could not copy file "<< src << " to " << dst;
            emit loadProjectError("Could not copy the DCIM files");
            return false;
        }
        i++;
    }

    this->front = QDir(projectDir.absolutePath() + "/DCIM/100GFRNT");
    this->back = QDir(projectDir.absolutePath() + "/DCIM/100GBACK");

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

        video->setFrontThumbnail(new QFile(front.path() + "/GPFR" + video->getIdString() + ".THM"));
        video->setBackThumbnail(new QFile(back.path() + "/GPBK" + video->getIdString() + ".THM"));

        if (!video->verify()) {
            badVideos.append({vid, "Invalid thumnails"});
            continue;
        }

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
                new QFile(front.path() + "/GPFR" + video->getIdString() + ".THM"),
                new QFile(back.path() + "/GB" + segIdString + video->getIdString() + ".MP4"),
                new QFile(back.path() + "/GB" + segIdString + video->getIdString() + ".LRV"),
                new QFile(back.path() + "/GPBK" + video->getIdString() + ".THM"),
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
    int chunks = 0;

    QElapsedTimer tmr;
    tmr.start();
    qint64 bytesStamp = 0;
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
        emit loadProjectUpdate(progress);
        if (tmr.elapsed() >= 1000) {
            double speed = (double) (progress.copy.currentFile.bytesDone - bytesStamp) / (double) (1024*1024);
            progress.copy.currentFile.speed = speed;
            emit loadProjectUpdate(progress);
            bytesStamp = progress.copy.currentFile.bytesDone;
            tmr.restart();
        }
    }

    srcFile.close();
    dstFile.close();

    return true;
}
