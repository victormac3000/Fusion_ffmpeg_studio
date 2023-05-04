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
            if (obj.contains("name") && obj.contains("firmwareVersion") && obj.contains("height") &&
                obj.contains("width") && obj.contains("fps")) {
                FFormat format;
                format.name = obj.value("name").toString();
                format.firmwareVersion = obj.value("firmwareVersion").toDouble();
                format.height = obj.value("height").toInt();
                format.width = obj.value("width").toInt();
                format.fps = obj.value("fps").toDouble();
                this->formats.append(format);
            }
        }
    }

    if (this->formats.length() < 1) {
        qCritical() << "No supported video formats defined in the FusionFormats.json file";
    }
}

FFormat* FFormats::get(QFile *video)
{
    QMediaPlayer player;
    player.setSource(QUrl(video->fileName()));
    QMediaMetaData videoMetadata = player.metaData();

    QSize resolution = videoMetadata.value(QMediaMetaData::Resolution).toSize();
    float fps = videoMetadata.value(QMediaMetaData::VideoFrameRate).toFloat();

    qDebug() << "File:" <<  video->fileName() << "Resolution:" << QString::number(resolution.height()) + "x" + QString::number(resolution.width()) << "FPS:" << fps;

    for (FFormat &format: formats) {
        if (format.height == resolution.height() && format.width == resolution.width() && format.fps == fps) {
            return &format;
        }
    }
    return nullptr;
}
