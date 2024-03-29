#include "fformats.h"

FFormats::FFormats(QObject *parent)
    : QObject{parent}
{
    QFile fusionFormatsFile(":/Documents/Json/FusionFormats.json");
    if (!fusionFormatsFile.open(QFile::ReadOnly)) {
        qWarning() << "Could not open the fusion formats file from resources, " << fusionFormatsFile.error();
        return;
    }
    QJsonParseError docError;
    QJsonDocument doc = QJsonDocument::fromJson(fusionFormatsFile.readAll(), &docError);
    fusionFormatsFile.close();
    if (docError.error != docError.NoError) {
        qWarning() << "Could not parse the fusion formats file from resources, " << docError.errorString();
        return;
    }
    if (!doc.isArray()) {
        qWarning() << "Could not parse the fusion formats file from resources, the main document must be an array";
        return;
    }
    QJsonArray mainArray = doc.array();
    for (QJsonValueRef objCandidate: mainArray) {
        if (objCandidate.isObject()) {
            QJsonObject obj = objCandidate.toObject();
            if (obj.contains("name") && obj.contains("firmwareVersion") && obj.contains("video") &&
                obj.contains("lowVideo") && obj.contains("thumnail")) {
                if (!obj.value("video").isObject() || !obj.value("lowVideo").isObject() || !obj.value("thumnail").isObject()) {
                    qWarning() << "Could not parse the fusion formats file from resources, video, lowVideo and thumnail must be objects";
                    return;
                }
                FFormat format;
                format.name = obj.value("name").toString();
                format.firmwareVersion = obj.value("firmwareVersion").toDouble();
                format.nVideo.height = obj.value("video").toObject().value("height").toInt();
                format.nVideo.width = obj.value("video").toObject().value("width").toInt();
                format.nVideo.fps = obj.value("video").toObject().value("fps").toDouble();
                format.lVideo.height = obj.value("lowVideo").toObject().value("height").toInt();
                format.lVideo.width = obj.value("lowVideo").toObject().value("width").toInt();
                format.lVideo.fps = obj.value("lowVideo").toObject().value("fps").toDouble();
                format.thumbnail.height = obj.value("thumnail").toObject().value("height").toInt();
                format.thumbnail.width = obj.value("thumnail").toObject().value("width").toInt();
                this->formats.append(format);
            }
        }
    }

    if (this->formats.length() < 1) {
        qCritical() << "No supported video formats defined in the FusionFormats.json file";
    }
}

FFormat* FFormats::get(QFile *media, int type)
{
    if (!media->exists()) {
        qWarning() << "The media specified does not exist: " << media->fileName();
        return nullptr;
    }

    if (type == FUSION_VIDEO || type == FUSION_LOW_VIDEO) {
        if (!MediaInfo::isVideo(media)) {
            qWarning() << "The media is not a video: " << media->fileName();
            return nullptr;
        }

        VideoInfo videoInfo = MediaInfo::getVideoInfo(media);

        for (FFormat &format: formats) {
            if (type == FUSION_VIDEO) {
                if (format.nVideo.height == videoInfo.resolution.height() &&
                    format.nVideo.width == videoInfo.resolution.width() &&
                    format.nVideo.fps == videoInfo.frameRate) {
                    return &format;
                }
            }
            if (type == FUSION_LOW_VIDEO) {
                if (format.lVideo.height == videoInfo.resolution.height() &&
                    format.lVideo.width == videoInfo.resolution.width() &&
                    format.lVideo.fps == videoInfo.frameRate) {
                    return &format;
                }
            }
        }
    }

    if (type == FUSION_AUDIO) {
        if (!MediaInfo::isAudio(media)) {
            qWarning() << "The media is not an audio file: " << media->fileName();
            return nullptr;
        }

        return &formats.first();
    }

    if (type == FUSION_THUMNAIL) {
        if (!MediaInfo::isImage(media)) {
            qWarning() << "The media is not a fusion thumnail: " << media->fileName();
            return nullptr;
        }

        QSize resolution = MediaInfo::getImageResolution(media);

        for (FFormat &format: formats) {
            if (format.thumbnail.height == resolution.height() &&
                format.thumbnail.width == resolution.width()) {
                return &format;
            }
        }
    }

    qWarning() << "The media type specified" << type << "is not an audio, video or thumnail file: " << media->fileName();
    return nullptr;
}
