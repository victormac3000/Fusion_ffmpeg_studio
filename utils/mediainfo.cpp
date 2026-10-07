#include "mediainfo.h"
#include "utils/settings.h"

#include <QElapsedTimer>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

VideoInfo MediaInfo::getVideoInfo(QString path)
{
    VideoInfo videoInfo = VideoInfo{};

    if (!QFile::exists(path)) {
        return videoInfo;
    }

    if (getMimeType(path).preferredSuffix() != "mp4") {
        return videoInfo;
    }
    videoInfo.isVideo = true;

    AVFormatContext* formatContext = nullptr;
    int ret;

    ret = avformat_open_input(&formatContext, path.toUtf8().constData(), nullptr, nullptr);
    if (ret < 0) {
        qWarning() << "FFMpeg could not open media file:" << path;
        return videoInfo;
    }

    ret = avformat_find_stream_info(formatContext, nullptr);
    if (ret < 0) {
        qWarning() << "FFMpeg could not find stream information:" << path;
        avformat_close_input(&formatContext);
        return videoInfo;
    }

    for (unsigned int i = 0; i < formatContext->nb_streams; ++i) {
        AVStream* stream = formatContext->streams[i];
        const AVCodecParameters *codecPar = stream->codecpar;

        if (codecPar->codec_type != AVMEDIA_TYPE_VIDEO)
            continue;

        const AVCodec* codec = avcodec_find_decoder(codecPar->codec_id);

        videoInfo.codecName = codec->name;
        videoInfo.codecLongName = codec->name;
        videoInfo.resolution = QSize(codecPar->width, codecPar->height);

        AVRational frameRate = stream->avg_frame_rate;
        if (frameRate.num != 0 && frameRate.den != 0) {
            videoInfo.frameRate = av_q2d(frameRate);
        }

        AVRational timeBase = stream->time_base;
        if (stream->duration != AV_NOPTS_VALUE) {
            videoInfo.length_s = stream->duration * av_q2d(timeBase);
        }
    }

    return videoInfo;
}

AudioInfo MediaInfo::getAudioInfo(QString path)
{
    if (!QFile::exists(path)) {
        return {};
    }
    return {
        .isAudio = getMimeType(path).preferredSuffix() == "wav"
    };
}

ImageInfo MediaInfo::getImageInfo(QString path)
{
    ImageInfo imageInfo;

    if (!QFile::exists(path)) {
        return imageInfo;
    }

    if (getMimeType(path).preferredSuffix() != "jpg") {
        return imageInfo;
    }
    imageInfo.isImage = true;

    QImage img(path);
    imageInfo.size = QSize(img.width(), img.height());

    return imageInfo;
}

QMimeType MediaInfo::getMimeType(QString path)
{
    QMimeDatabase db;
    QMimeType type = db.mimeTypeForFile(path, QMimeDatabase::MatchContent);
    return type;
}