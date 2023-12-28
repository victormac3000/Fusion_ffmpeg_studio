#include "ffmpeg.h"
#include "models/project.h"

FFmpeg::FFmpeg(QObject *parent)
    : QObject{parent}
{
    this->process = new QProcess(this);

    connect(process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(processErrorOccurred(QProcess::ProcessError)));
    connect(process, SIGNAL(readyReadStandardError()), this, SLOT(processReadyReadError()));
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(processReadyReadOut()));
    connect(process, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(processStateChanged(QProcess::ProcessState)));

    QString ffmpegPath = QSettings().value("ffmpeg").toString();

    if (ffmpegPath.isEmpty()) {
        qWarning() << "FFMpeg binary path not found:" << ffmpegPath;
    }

    this->process->setProgram(ffmpegPath);

    qDebug() << "FFMpeg created";
}

FFmpeg::~FFmpeg()
{
    process->terminate();
    qDebug() << "FFmpeg running, waiting for finished";
    process->waitForFinished();
    delete process;
    qDebug() << "FFMpeg destroyed";
}

void FFmpeg::render(RenderWork *work)
{
    QString projectPath = work->getProject()->getPath();

    dfLowSegmentsFolder = QDir(projectPath + "/DFLowSegments");
    dfLowVideosFolder = QDir(projectPath + "/DFLowVideos");
    dfSegmentsFolder = QDir(projectPath + "/DFSegments");
    dfVideosFolder = QDir(projectPath + "/DFVideos");
    equirectangularVideosFolder = QDir(projectPath + "/EVideos");
    equirectangularLowVideosFolder = QDir(projectPath + "/ELowVideos");

    if (!dfLowSegmentsFolder.exists() || !dfLowVideosFolder.exists() || !dfSegmentsFolder.exists() ||
        !dfVideosFolder.exists() || !equirectangularVideosFolder.exists() || !equirectangularLowVideosFolder.exists()) {
        qWarning() << "The project directory does not have the required directories" << projectPath;
        emit renderDone(work, true);
        return;
    }

    this->work = work;

    if (work->getType() == RENDER_PREVIEW) {
        renderPreview();
        emit renderDone(work, false);
    } else {
        emit renderDone(work, false);
    }
}

void FFmpeg::processStateChanged(QProcess::ProcessState newState)
{
    qDebug() << "FFmpeg has changed state:" << newState;
}

void FFmpeg::processReadyReadOut()
{
    qDebug() << "FFmpeg an output:" << process->readAllStandardOutput();
}

void FFmpeg::processReadyReadError()
{
    QString standardError = QString::fromUtf8(process->readAllStandardError());

    QRegularExpression statusRe("frame=\\s*(?<nframe>[0-9]+)\\s+fps=\\s*(?<nfps>[0-9\\.]+)\\s+q=(?<nq>[0-9\\.-]+)\\s+(L?)\\s*size=\\s*(?<nsize>[0-9]+)(?<ssize>kB|mB|b)?\\s*time=\\s*(?<sduration>[0-9\\:\\.]+)\\s*bitrate=\\s*(?<nbitrate>[0-9\\.]+)(?<sbitrate>bits\\/s|mbits\\/s|kbits\\/s)?.*(dup=(?<ndup>\\d+)\\s*)?(drop=(?<ndrop>\\d+)\\s*)?speed=\\s*(?<nspeed>[0-9\\.]+)x");

    QRegularExpressionMatch statusReMatch = statusRe.match(standardError);
    if (statusReMatch.hasMatch()) {
        bool frameOk = false;
        bool fpsOk = false;
        bool qualityOk = false;
        bool sizeOk = false;
        bool bitrateOk = false;
        bool speedOk = false;

        long frame = statusReMatch.captured("nframe").toLong(&frameOk);
        double fps = statusReMatch.captured("nfps").toDouble(&fpsOk);
        double quality = statusReMatch.captured("nq").toDouble(&qualityOk);
        long size = statusReMatch.captured("nsize").toLong(&sizeOk);
        QString time = statusReMatch.captured("sduration");
        double bitrate = statusReMatch.captured("nbitrate").toDouble(&bitrateOk);
        double speed = statusReMatch.captured("nspeed").toDouble(&speedOk);

        if (!frameOk) qWarning() << "Parameter frame of ffmpeg Output could not be read correctly. Full status: " << stderr;
        if (!fpsOk) qWarning() << "Parameter fps of ffmpeg Output could not be read correctly. Full status: " << stderr;
        if (!qualityOk) qWarning() << "Parameter q (quality) of ffmpeg Output could not be read correctly. Full status: " << stderr;
        if (!bitrateOk) qWarning() << "Parameter bitrate of ffmpeg Output could not be read correctly. Full status: " << stderr;
        if (!speedOk) qWarning() << "Parameter speed of ffmpeg Output could not be read correctly. Full status: " << stderr;

        struct FFmpegStatus status;

        status.frame = frame;
        status.fps = fps;
        status.quality = quality;
        status.size = size;
        status.elapsedTime = QTime::fromString(time);
        status.bitrate = bitrate;
        status.speed = speed;
        status.percent = 0.55f;

        emit work->updateRenderStatus(&status);

        qInfo() << "FFmpeg status: "
                << "FRAME (" << frame << ") "
                << "FPS (" << fps << ") "
                << "QUALITY (" << quality << ") "
                << "SIZE (" << size << ") "
                << "TIME (" << time << ") "
                << "BITRATE (" << bitrate << ") "
                << "SPEED (" << speed << ") ";

    } else {
        qWarning() << "FFmpeg has given an stdout error:" << stderr;
    }
}

void FFmpeg::processErrorOccurred(QProcess::ProcessError error)
{
    qWarning() << "FFmpeg has given an error:" << error;
}

void FFmpeg::renderPreview()
{
    FVideo *video = work->getVideo();
    qDebug() << "Generating RENDER_PREVIEW for video " << video->getId();
    if (renderPreviewStep1()) return;
    if (renderPreviewStep2()) return;
    if (renderPreviewStep3()) return;
}

bool FFmpeg::renderPreviewStep1()
{
    // STEP 1: MERGE ALL SEGMENTS FRONT AND BACK LOW RES VIDEOS
    emit work->updateRenderStatusString("Step 1 of 3: Merging front and back of each segment");
    FVideo *video = work->getVideo();
    int numSegments = video->getNumSegments();

    for (int i=0; i<numSegments; i++) {
        FSegment *segment = video->getSegment(i);

        qDebug() << "Merging front and back of segment" << segment->getId();

        if (segment->isDualFisheyeLowValid()) {
            qDebug() << "The segment had already a merged dual fisheye video";
            if (!work->getOverwrite()) {
                qDebug() << "The work specified not to overwrite the existing video, we can skip this segment";
                continue;
            }
        }

        QString outPath = dfLowSegmentsFolder.absolutePath()
                          + "/"
                          + QString::number(video->getId())
                          + "_"
                          + QString::number(segment->getId())
                          + ".MP4";

        if (QFile::exists(outPath) && !QFile::remove(outPath)) {
            qWarning() << "Could not override the existing dual fisheye video of segment" << segment->getId() << "on path" << outPath;
            emit renderDone(work, true);
            return true;
        }

        QList<QString> params;

        params.append("-hide_banner");
        params.append("-loglevel");
        params.append("error");
        params.append("-stats");
        params.append("-i");
        params.append(QDir::toNativeSeparators(segment->getFrontLRV()->fileName()));
        params.append("-i");
        params.append(QDir::toNativeSeparators(segment->getBackLRV()->fileName()));
        params.append("-filter_complex");
        params.append("hstack");
        params.append("-c:v");
        params.append("hevc_videotoolbox");
        params.append(QDir::toNativeSeparators(outPath));


        if (process->state() == QProcess::Running) {
            qDebug() << "FFMpeg is busy, Cannot do work of type" << work->getTypeString() << "for video" << work->getVideo()->getIdString();
            return true;
        }

        process->setArguments(params);
        process->start();
        process->waitForFinished(-1);

        qDebug() << "FFMpeg process finished";

        if (process->exitCode() > 0) {
            qWarning() << "FFMpeg exited with error code " << process->exitCode()
                       << " and exit status" << process->exitStatus()
                       << "Could not generate the dual fisheye video of segment"
                       << segment->getId() << "on path" << outPath << "The stdout was "
                       << process->readAllStandardError();
            emit renderDone(work, true);
            return true;
        }

        segment->setDualFisheyeLow(new QFile(outPath));
    }
    return false;
}

bool FFmpeg::renderPreviewStep2()
{
    // STEP 2: MERGE ALL SEGMENTS INTO ONE BIG LRV VIDEO
    emit work->updateRenderStatusString("Step 2 of 3: Merging all preview segments into one preview video");
    FVideo *video = work->getVideo();

    qDebug() << "Merging all segments into one video";

    if (video->isDualFisheyeLowValid()) {
        qDebug() << "The segment had already a merged dual fisheye preview video";
        if (!work->getOverwrite()) {
            qDebug() << "The work specified not to overwrite the existing merged dual fisheye video, we can skip this step";
            return false;
        }
    }

    QString outPath = dfLowVideosFolder.absolutePath()
                      + "/"
                      + QString::number(video->getId())
                      + ".MP4";

    if (QFile::exists(outPath) && !QFile::remove(outPath)) {
        qWarning() << "Could not override the existing dual fisheye preview video of segments located in" << outPath;
        emit renderDone(work, true);
        return true;
    }

    QString concatStr;
    int numSegments = video->getNumSegments();
    for (int i=0; i<numSegments; i++) {
        concatStr.append("file '" + QDir::toNativeSeparators(video->getSegment(i)->getDualFisheyeLow()->fileName()) + "'");
        concatStr.append("\n");
    }

    QFile concatFile(settings.value("appData").toString() + "/concat.txt");
    if (!concatFile.open(QFile::ReadWrite) || !concatFile.write(concatStr.toUtf8())) {
        qWarning() << "Could not create the required file for concatenating segments on" << concatFile.fileName();
        emit renderDone(work, true);
        return true;
    }
    concatFile.close();

    QList<QString> params;

    params.append("-hide_banner");
    params.append("-loglevel");
    params.append("error");
    params.append("-stats");
    params.append("-f");
    params.append("concat");
    params.append("-safe");
    params.append("0");
    params.append("-i");
    params.append(QDir::toNativeSeparators(concatFile.fileName()));
    params.append("-c");
    params.append("copy");
    params.append(QDir::toNativeSeparators(outPath));


    if (process->state() == QProcess::Running) {
        qWarning() << "FFMpeg is busy, Cannot do work of type" << work->getTypeString() << "for video" << work->getVideo()->getIdString();
        emit renderDone(work, true);
        return true;
    }

    process->setArguments(params);
    process->start();
    process->waitForFinished(-1);

    concatFile.remove();

    if (process->exitCode() > 0) {
        qWarning() << "FFMpeg exited with error code >0" << process->exitCode()
                   << "and exit status" << process->exitStatus()
                   << "Could not generate the dual fisheye merged preview video of all segments"
                   << "on path" << outPath;
        emit renderDone(work, true);
        return true;
    }
    video->setDualFisheyeLow(new QFile(outPath));
    return false;
}

bool FFmpeg::renderPreviewStep3()
{
    // STEP 3: CONVERT DUAL FISHEYE PREVIEW INTO A EQUIRECTANGULAR PREVIEW
    emit work->updateRenderStatusString("Step 3 of 3: Converting dual fisheye preview to equirectangular");
    FVideo *video = work->getVideo();

    qDebug() << "Converting dual fisheye preview of video" << video->getId() << "to equirectangular";

    if (video->isEquirectangularLowValid()) {
        qDebug() << "The segment had already an equirectangular preview video";
        if (!work->getOverwrite()) {
            qDebug() << "The work specified not to overwrite the existing equirectangular preview video, we can skip this step";
            return false;
        }
    }

    QString outPath = equirectangularLowVideosFolder.absolutePath()
              + "/"
              + QString::number(video->getId())
              + ".MP4";

    if (QFile::exists(outPath) && !QFile::remove(outPath)) {
        qWarning() << "Could not override the existing equirectangular preview video of segments located in" << outPath;
        emit renderDone(work, true);
        return true;
    }

    QList<QString> params;

    params.append("-hide_banner");
    params.append("-loglevel");
    params.append("error");
    params.append("-stats");
    params.append("-y");
    params.append("-i");
    params.append(QDir::toNativeSeparators(video->getDualFisheyeLow()->fileName()));
    params.append("-vf");
    params.append("v360=dfisheye:equirect:ih_fov=190:iv_fov=190");
    params.append("-c:v");
    params.append("hevc_videotoolbox");
    params.append(QDir::toNativeSeparators(outPath));


    if (process->state() == QProcess::Running) {
        qDebug() << "FFMpeg is busy, Cannot do work of type" << work->getTypeString() << "for video" << work->getVideo()->getIdString();
        return true;
    }

    process->setArguments(params);
    process->start();
    process->waitForFinished(-1);

    qDebug() << "FFMpeg process finished end wait finished";

    if (process->exitCode() > 0) {
        qWarning() << "FFMpeg exited with error code >0" << process->exitCode()
                   << "and exit status" << process->exitStatus()
                   << "Could not generate the equirectangular preview video of all segments"
                   << "on path" << outPath;
        emit renderDone(work, true);
        return true;
    }
    video->setEquirectangularLow(new QFile(outPath));
    return false;
}
