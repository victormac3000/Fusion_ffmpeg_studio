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

    QString ffmpegPath = getPath();

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

    QDir dfLowSegmentsFolder(projectPath + "/DFLowSegments");
    QDir dfLowVideosFolder(projectPath + "/DFLowVideos");
    QDir dfSegmentsFolder(projectPath + "/DFSegments");
    QDir dfVideosFolder(projectPath + "/DFVideos");

    if (!dfLowSegmentsFolder.exists() || !dfLowVideosFolder.exists() || !dfSegmentsFolder.exists() || !dfVideosFolder.exists()) {
        qWarning() << "The project directory does not have the required directories" << projectPath;
        emit renderDone(work, true);
        return;
    }

    this->work = work;
    FVideo *video = work->getVideo();
    int numSegments = video->getNumSegments();

    if (RENDER_PREVIEW) {
        qDebug() << "Generating RENDER_PREVIEW for video " << video->getId();

        // STEP 1: MERGE ALL SEGMENTS FRONT AND BACK LOW RES VIDEOS
        emit work->updateRenderStatusString("Step 1 of 2: Merging front and back of each segment");

        for (int i=0; i<numSegments; i++) {
            FSegment *segment = video->getSegment(i);

            qDebug() << "Merging front and back of segment" << segment->getId();

            if (segment->getDualFisheyeLow() != nullptr && segment->getDualFisheyeLow()->exists()) {
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
                return;
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
            params.append("libx265");
            params.append(QDir::toNativeSeparators(outPath));


            if (process->state() == QProcess::Running) {
                qDebug() << "FFMpeg is busy, Cannot do work of type" << work->getTypeString() << "for video" << work->getVideo()->getIdString();
                return;
            }

            process->setArguments(params);
            process->start();

            while (process->state() == QProcess::Running) {}

            qDebug() << "FFMpeg process finished";

            if (process->exitCode() > 0) {
                qWarning() << "FFMpeg exited with error code >0" << process->exitCode()
                           << "and exit status" << process->exitStatus()
                           << "Could not generate the dual fisheye video of segment"
                           << segment->getId() << "on path" << outPath;
                emit renderDone(work, true);
                return;
            }

            segment->setDualFisheye(new QFile(outPath));
        }
        // STEP 2: MERGE ALL SEGMENTS INTO ONE BIG LRV VIDEO
        // STEP 3: LINK THE BIG LRV VIDEO TO THE FVIDEO OBJECT
    }





    emit renderDone(work, false);
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
    QString stderr = process->readAllStandardError();

    static QRegularExpression statusRe("frame=\\s*(?<nframe>[0-9]+)\\s+fps=\\s*(?<nfps>[0-9\\.]+)\\s+q=(?<nq>[0-9\\.-]+)\\s+(L?)\\s*size=\\s*(?<nsize>[0-9]+)(?<ssize>kB|mB|b)?\\s*time=\\s*(?<sduration>[0-9\\:\\.]+)\\s*bitrate=\\s*(?<nbitrate>[0-9\\.]+)(?<sbitrate>bits\\/s|mbits\\/s|kbits\\/s)?.*(dup=(?<ndup>\\d+)\\s*)?(drop=(?<ndrop>\\d+)\\s*)?speed=\\s*(?<nspeed>[0-9\\.]+)x");

    QRegularExpressionMatch statusReMatch = statusRe.match(stderr);
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
        status.percent = 0.55;

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

QString FFmpeg::getPath()
{
    QString resourcesPath;
    QString extension;

    #ifdef Q_OS_MAC
        resourcesPath = ":/Binaries/macOS/ffmpeg";
    #endif

    #ifdef Q_OS_WIN
        extension = ".exe";
        resourcesPath = ":/Binaries/windows/ffmpeg.exe";
    #endif

    #ifdef Q_OS_Q_OS_LINUX
        resourcesPath = ":/Binaries/linux/ffmpeg";
    #endif

    if (resourcesPath.isEmpty()) {
        qWarning() << "No resources found for unknown OS";
        return resourcesPath;
    }

    QDir workingDir(settings.value("workingDir").toString());

    if (!workingDir.exists()) {
        qWarning() << "The working directory does not exist:" <<  workingDir.absolutePath();
        return "";
    }

    if (!workingDir.exists("Binaries")) {
        if (!workingDir.mkdir("Binaries")) {
            qWarning() << "Could not create the binaries directory inside the working directory:" << workingDir.absolutePath();
            return "";
        }
    }

    if (!workingDir.cd("Binaries")) {
        qWarning() << "Could not move to the binaries directory inside the working directory:" << workingDir.absolutePath();
        return "";
    }

    QFile resourcesFile(resourcesPath);
    QString localFilePath(workingDir.absolutePath() + "/ffmpeg" + extension);

    if (!QFile::exists(localFilePath)) {
        if (!resourcesFile.copy(localFilePath)) {
            qWarning() << "Could not copy the binary from the resources path:" << resourcesFile.fileName() <<
                "to the binaries path of the working directory:" << workingDir.absolutePath();
            return "";
        }
    }

    if (!QFile(localFilePath).setPermissions(
        QFileDevice::ReadOwner |  QFileDevice::ReadUser |  QFileDevice::ReadGroup | QFileDevice::ReadOther |
        QFileDevice::WriteOwner |
        QFileDevice::ExeOwner |  QFileDevice::ExeUser |  QFileDevice::ExeGroup | QFileDevice::ExeOther)) {
        qWarning() << "Could not set the binary permissions:" << localFilePath;
        return "";
    }

    return localFilePath;
}
