#include "fsegment.h"
#include "fvideo.h"
#include "utils/fformats.h"
#include "utils/mediainfo.h"
#include "utils/toolbox.h"

FVideo::FVideo(int id)
{
    this->id = id;
    this->name = getIdString();
}

FVideo FVideo::fromRow(const QVariant& row, QString projectDirPath)
{
    QVariantMap map = row.toMap();

    FVideo instance(map.value("id").toInt());

    instance.setName(map.value("name").toString());

    QStringList mediaList = {
        map.value("frontThumbnail").toString(),
        map.value("backThumbnail").toString()
    };

    if (!projectDirPath.isEmpty()) {
        for (int i=0; i<mediaList.length(); i++) {
            mediaList[i] = Toolbox::toAbsolutePath(mediaList[i], projectDirPath);
        }
    }

    instance.setFrontThumbnail(mediaList[0]);
    instance.setBackThumbnail(mediaList[1]);

    FFormat format {
        .name = map.value("format_name").toString(),
        .firmware = map.value("format_firmware").toString(),
        .height = map.value("format_height").toInt(),
        .width = map.value("format_width").toInt(),
        .fps = map.value("format_fps").toDouble()
    };

    instance.setFormat(format);

    return instance;
}

int FVideo::getId()
{
    return this->id;
}

QString FVideo::getIdString()
{
    QString vidIdString = QString::number(id);
    while (vidIdString.length() < 4) vidIdString.insert(0, "0");
    return vidIdString;
}

bool FVideo::setFrontThumbnail(QString thumbnailPath)
{
    FFormat format = FFormats::get(thumbnailPath, FUSION_THUMNAIL);
    if (format.name.isEmpty()) {
        qWarning() << "Tried to set an invalid front thumbnail for video"
                   << getIdString();
        return false;
    }

    this->frontThumbnail = thumbnailPath;
    return true;
}

bool FVideo::setBackThumbnail(QString thumbnailPath)
{
    FFormat format = FFormats::get(thumbnailPath, FUSION_THUMNAIL);
    if (format.name.isEmpty()) {
        qWarning() << "Tried to set an invalid back thumbnail for video"
                   << getIdString();
        return false;
    }

    this->backThumbnail = thumbnailPath;
    return true;
}

void FVideo::setFormat(FFormat format)
{
    this->format = format;
}

void FVideo::setName(QString name)
{
    this->name = name;
}

bool FVideo::addSegment(FSegment segment)
{
    if (this->id != segment.getVideoId()) {
        qDebug() << "Tried to add a segment that does not belong to this video";
        return false;
    }
    this->segments.append(segment);
    return true;
}

QString FVideo::getFrontThumbnail()
{
    return this->frontThumbnail;
}

QString FVideo::getBackThumbnail()
{
    return this->backThumbnail;
}

QList<FSegment> FVideo::getSegments()
{
    return segments;
}

FFormat FVideo::getFormat()
{
    return this->format;
}

QVariantMap FVideo::toVariantMap(QString projectDirPath)
{
    VideoInfo firstSegmentInfo = this->segments.first().getFrontVideoInfo();

    QDateTime recordedOn = firstSegmentInfo.date;
    QString durationString = firstSegmentInfo.getLengthS();

    QVariantMap map;

    QString frontThumbnail = this->frontThumbnail;
    QString backThumbnail = this->backThumbnail;

    if (!projectDirPath.isEmpty()) {
        frontThumbnail = Toolbox::toRelativePath(frontThumbnail, projectDirPath);
        backThumbnail = Toolbox::toRelativePath(backThumbnail, projectDirPath);
    }

    // Stored
    map.insert("id", id);
    map.insert("name", name);

    map.insert("frontThumbnail", frontThumbnail);
    map.insert("backThumbnail", backThumbnail);

    map.insert("format_name", format.name);
    map.insert("format_firmware", format.firmware);
    map.insert("format_height", format.height);
    map.insert("format_width", format.width);
    map.insert("format_fps", format.fps);

    // Calculated

    map.insert("idString", this->getIdString());

    map.insert("front", this->segments.first().getFrontVideo());
    map.insert("frontURL", QUrl::fromLocalFile(this->segments.first().getFrontVideo()));

    map.insert("back", this->segments.first().getBackVideo());
    map.insert("backURL", QUrl::fromLocalFile(this->segments.first().getBackVideo()));

    map.insert("recordedOn", recordedOn);
    map.insert("durationString", durationString);

    return map;
}