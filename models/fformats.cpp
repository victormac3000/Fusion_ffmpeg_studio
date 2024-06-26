#include "fformats.h"

FFormats::FFormats(QObject *parent)
    : QObject{parent}
{
    formats.append(FFormat {"3K@50", 1504, 1568, 50} );

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
                if (format.height == videoInfo.resolution.height() &&
                    format.width == videoInfo.resolution.width() &&
                    format.fps == videoInfo.frameRate) {
                    return &format;
                }
            }
            if (type == FUSION_LOW_VIDEO) {
                if (lowVideoHeight == videoInfo.resolution.height() &&
                    lowVideoWidth == videoInfo.resolution.width() &&
                    lowVideoFps == videoInfo.frameRate) {
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
            if (thumbHeight == resolution.height() &&
                thumbWidth == resolution.width()) {
                return &format;
            }
        }
    }

    qWarning() << "The media type specified" << type << "is not an audio, video or thumnail file: " << media->fileName();
    return nullptr;
}
