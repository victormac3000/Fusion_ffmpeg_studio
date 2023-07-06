#include "ffmpeg.h"

FFmpeg::FFmpeg(QObject *parent)
    : QObject{parent}
{
    this->process = new QProcess(this);

    connect(process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(processErrorOccurred(QProcess::ProcessError)));
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int,QProcess::ExitStatus)));
    connect(process, SIGNAL(readyReadStandardError()), this, SLOT(processReadyReadError()));
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(processReadyReadOut()));
    connect(process, SIGNAL(started()), this, SLOT(processStarted()));
    connect(process, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(processStateChanged(QProcess::ProcessState)));

    connect(this, SIGNAL(renderDone(RenderWork*,bool)), this, SLOT(renderDone()));

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

bool FFmpeg::isRunning()
{
    return running;
}

void FFmpeg::render(RenderWork *work)
{
    QDir workingDir(settings.value("workingDir").toString());

    if (!workingDir.exists()) {
        qWarning() << "The working directory does not exist: " + workingDir.absolutePath();
        emit renderDone(work, true);
        return;
    }

    this->work = work;
    FVideo *video = work->getVideo();
    int numSegments = video->getNumSegments();

    if (RENDER_PREVIEW) {
        qDebug() << "Generating render preview for video " << video->getId();

        // STEP 1: MERGE ALL SEGMENTS FRONT AND BACK LOW RES VIDEOS
        emit work->updateRenderStatusString("Merging front and back segment");


        // STEP 2: MERGE ALL SEGMENTS INTO ONE BIG LRV VIDEO
        // STEP 3: LINK THE BIG LRV VIDEO TO THE FVIDEO OBJECT
    }




    for (int i=0; i<numSegments; i++) {
        FSegment *segment = video->getSegment(i);

        qDebug() << "Merging front and back of segment" << segment->getId();

        if (segment->getMerged() != nullptr) {
            qDebug() << "The segment had already a merged video. The new one will override it";
        }

        QString outPath = workingDir.absolutePath()
                          + "/"
                          + video->getIdString()
                          + "_"
                          + segment->getIdString()
                          + "_MERGE"
                          + ".LRV";

        QFile outFile(outPath);

        if (outFile.exists() && !outFile.remove()) {
            qWarning() << "Could not override the existing merged video of segment" << segment->getId() << "on path" << outPath;
            emit renderDone(work, true);
            return;
        }

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
        params.append("-f");
        params.append("mp4");
        params.append(QDir::toNativeSeparators(outPath));


        if (process->state() == QProcess::Running) {
            process->terminate();
            qDebug() << "FFmpeg running, waiting for finished";
            process->waitForFinished();
        }

        process->setArguments(params);
        process->start();

        while (running) {}

        if (exitCode > 0) {
            qWarning() << "FFMpeg exited with error code > 0 " << exitCode
                       << "Could not override the existing merged video of segment"
                       << segment->getId() << "on path" << outPath;
            emit renderDone(work, true);
            return;
        }
    }
    emit renderDone(work, false);
}

void FFmpeg::processStarted()
{
    running = true;
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

void FFmpeg::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    running = false;
    this->exitCode = (exitStatus == QProcess::CrashExit) ? 1001 : exitCode;
}

void FFmpeg::renderDone()
{
    running = false;
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
