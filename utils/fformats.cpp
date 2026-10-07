#include "fformats.h"

#include "utils/mediainfo.h"

QList<FFormat> FFormats::formats = {
    FFormat {"3K@50", "2.0", 1504, 1568, 50}
};

FFormat FFormats::get(QString mediaPath, int type)
{
    QFile media(mediaPath);

    if (!media.exists()) {
        qWarning() << "The media specified does not exist: " << media.fileName();
        return FFormat{};
    }

    if (type == FUSION_VIDEO || type == FUSION_LOW_VIDEO) {
        VideoInfo videoInfo = MediaInfo::getVideoInfo(media.fileName());
        if (!videoInfo.isVideo) {
            qWarning() << "The media is not a video: " << media.fileName();
            return FFormat{};
        }

        for (FFormat &format: formats) {
            if (type == FUSION_VIDEO) {
                if (format.height == videoInfo.resolution.height() &&
                    format.width == videoInfo.resolution.width() &&
                    format.fps == videoInfo.frameRate) {
                    return format;
                }
            }
            if (type == FUSION_LOW_VIDEO) {
                if (lowVideoHeight == videoInfo.resolution.height() &&
                    lowVideoWidth == videoInfo.resolution.width() &&
                    lowVideoFps == videoInfo.frameRate) {
                    return format;
                }
            }
        }
    }

    if (type == FUSION_AUDIO) {
        AudioInfo audioInfo = MediaInfo::getAudioInfo(media.fileName());
        if (!audioInfo.isAudio) {
            qWarning() << "The media is not an audio file: " << media.fileName();
            return FFormat{};
        }

        return formats.first();
    }

    if (type == FUSION_THUMNAIL) {
        ImageInfo imageInfo = MediaInfo::getImageInfo(media.fileName());
        if (!imageInfo.isImage) {
            qWarning() << "The media is not a fusion thumnail: " << media.fileName();
            return FFormat{};
        }

        for (FFormat& format: formats) {
            if (thumbHeight == imageInfo.size.height() &&
                thumbWidth == imageInfo.size.width()) {
                // Returns first format in order to check that is ok
                return format;
            }
        }
    }

    qWarning() << "The media type specified" << type << "is not an audio, video or thumnail file: " << media.fileName();
    return FFormat{};
}

FFormat FFormats::getByName(QString name)
{
    for (FFormat& format: formats) {
        if (format.name == name) {
            return format;
        }
    }
    return FFormat{};
}
