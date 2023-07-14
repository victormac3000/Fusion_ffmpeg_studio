#include "project.h"

Project::Project(QObject *parent, QString projectFilePath)
    : QObject{parent}
{
    this->file = new QFile(projectFilePath);

    if (!file->exists()) {
        qWarning() << "ProjectFile QFile is empty" << projectFilePath;
        return;
    }

    if (!file->open(QFile::ReadWrite | QFile::ExistingOnly)) {
        qWarning() << "Could not open project file" << projectFilePath;
        return;
    }

    QJsonParseError error;
    this->json = QJsonDocument::fromJson(file->readAll(), &error);

    if (error.error != QJsonParseError::NoError) {
        file->close();
        qWarning() << "Could not parse project file" << error.errorString();
        return;
    }

    if (!json.isObject()) {
        file->close();
        qWarning() << "The project file is not a json object";
        return;
    }

    QJsonObject mainObj = json.object();

    if (mainObj.value("info").toObject().value("dcim").toString().isEmpty()
        || mainObj.value("info").toObject().value("version").toString().isEmpty()) {
        file->close();
        qWarning() << "Info object: one of its children (info or version) not found or empty";
        return;
    }

    if (!mainObj.value("videos").isArray()) {
        file->close();
        qWarning() << "Videos object is not an array";
        return;
    }

    this->path = QFileInfo(file->fileName()).absolutePath();
    this->dcim = mainObj.value("info").toObject().value("dcim").toString();
    this->version = mainObj.value("info").toObject().value("version").toString();

    QJsonArray videosArray = mainObj.value("videos").toArray();

    for (const QJsonValue &videoArray: videosArray) {
        bool valid = true;
        if (!videoArray.isObject()) continue;
        QJsonObject videoObject = videoArray.toObject();
        int vid = videoObject.value("id").toInt(-1);
        if (vid<0) continue;
        if (!videoObject.value("segments").isArray()) continue;
        QJsonArray segmentsArray = videoObject.value("segments").toArray();
        if (segmentsArray.isEmpty()) continue;
        FVideo *video = new FVideo(nullptr, vid);
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
                    new QFile(dcim + "/100GFRNT/GPFR" + video->getIdString() + ".MP4"),
                    new QFile(dcim + "/100GFRNT/GPFR" + video->getIdString() + ".LRV"),
                    new QFile(dcim + "/100GFRNT/GPFR" + video->getIdString() + ".THM"),
                    new QFile(dcim + "/100GBACK/GPBK" + video->getIdString() + ".MP4"),
                    new QFile(dcim + "/100GBACK/GPBK" + video->getIdString() + ".LRV"),
                    new QFile(dcim + "/100GBACK/GPBK" + video->getIdString() + ".THM"),
                    new QFile(dcim + "/100GBACK/GPBK" + video->getIdString() + ".WAV")
                );
            } else {
                segment = new FSegment(
                    video, sid,
                    new QFile(dcim + "/100GFRNT/GF" + sidString + video->getIdString() + ".MP4"),
                    new QFile(dcim + "/100GFRNT/GF" + sidString + video->getIdString() + ".LRV"),
                    new QFile(dcim + "/100GFRNT/GPFR" + video->getIdString() + ".THM"),
                    new QFile(dcim + "/100GBACK/GB" + sidString + video->getIdString() + ".MP4"),
                    new QFile(dcim + "/100GBACK/GB" + sidString + video->getIdString() + ".LRV"),
                    new QFile(dcim + "/100GBACK/GPBK" + video->getIdString() + ".THM"),
                    new QFile(dcim + "/100GBACK/GB" + sidString + video->getIdString() + ".WAV")
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
            if (!video->addSegment(segment)) {
                qDebug() << "Segment invalid" << sid << "for video" << vid;
                valid = false;
                break;
            }
        }
        if (valid) this->videos.append(video);
    }

    if (!QDir(path).exists("DFSegments") || !QDir(path).exists("DFVideos") ||
        !QDir(path).exists("DFLowSegments") || !QDir(path).exists("DFLowVideos")) {
        file->close();
        qWarning() << "Project folder invalid, required folders not found" << path;
        return;
    }

    file->close();
    this->valid = true;
}

Project::~Project()
{
    delete file;
}

bool Project::isValid()
{
    return valid;
}

QString Project::getDcim()
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

void Project::setPath(QString newPath)
{
    path = newPath;
}

QFile* Project::getFile()
{
    return file;
}

void Project::setFile(QFile *newFile)
{
    file = newFile;
}

void Project::setVideos(QList<FVideo*> videos)
{
    this->videos = videos;
}

QList<FVideo*> Project::getVideos()
{
    return videos;
}

void Project::save()
{
    qDebug() << "Trying to save to project file" << file->fileName();

    if (!file->open(QFile::ReadWrite | QFile::ExistingOnly)) {
        file->close();
        qWarning() << "Could not open project file" << path;
        return;
    }

    QJsonDocument newJson;
    QJsonObject mainObj;

    QJsonObject info = json.object().value("info").toObject();

    info.insert("dcim", dcim);
    info.insert("version", QCoreApplication::applicationVersion());

    mainObj.insert("info", info);

    QJsonArray videosArray;

    for (FVideo *video: videos) {
        QJsonObject videoObject;
        videoObject.insert("id", video->getId());
        QJsonArray segmentsArray;
        QList<FSegment*> segments = video->getSegments();
        for (FSegment* segment: segments) {
            QJsonObject segmentObject;
            segmentObject.insert("id", segment->getId());
            segmentObject.insert("dualFisheye", segment->getDualFisheye() != nullptr);
            segmentObject.insert("dualFisheyeLow", segment->getDualFisheyeLow() != nullptr);
            segmentsArray.append(segmentObject);
        }
        videoObject.insert("segments", segmentsArray);
        videosArray.append(videoObject);
    }

    mainObj.insert("videos", videosArray);
    newJson.setObject(mainObj);

    int written = file->write(newJson.toJson(QJsonDocument::Indented));
    if (written < 0) {
        file->close();
        qWarning() << "Could not save the project" << path;
        return;
    }

    file->close();

    lastSaved = QDateTime::currentDateTime();
}
