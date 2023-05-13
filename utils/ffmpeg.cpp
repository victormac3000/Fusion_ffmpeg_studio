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

void FFmpeg::preRender(FVideo *video)
{
    /*
    command.replace("ffmpeg", fixPath(getPath()));
    command.replace("<front>", video->getSegment(0)->getFrontMP4()->fileName());
    command.replace("<back>", video->getSegment(0)->getBackMP4()->fileName());
    command.replace("<codec>", "libx265");
    command.replace("<merge>", settings.value("renderedDir").toString() + "/" + video->getIdString() + ".MP4");
    */

    QString outPath = settings.value("workingDir").toString() + "/" + video->getIdString() + ".MP4";
    QFile outFile(outPath);

    if (outFile.exists() && !outFile.remove()) {
        emit preRenderError(video, "There was an internal error, inspect the logs for more information");
        qWarning() << "Could not override the output file" << outPath;
        return;
    }

    params.append("-hide_banner");
    params.append("-loglevel");
    params.append("error");
    params.append("-stats");
    params.append("-i");
    params.append(QDir::toNativeSeparators(video->getSegment(0)->getFrontMP4()->fileName()));
    params.append("-i");
    params.append(QDir::toNativeSeparators(video->getSegment(0)->getBackMP4()->fileName()));
    params.append("-filter_complex");
    params.append("hstack");
    params.append("-c:v");
    params.append("libx265");
    params.append(QDir::toNativeSeparators(outPath));


    if (process->state() == QProcess::Running) {
        process->terminate();
        qDebug() << "FFmpeg running, waiting for finished";
        process->waitForFinished();
    }

    process->setArguments(params);
    process->start();
}

void FFmpeg::processStarted()
{
    qDebug() << "FFmpeg has started";
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

    static QRegularExpression re("frame=\\s*(?<nframe>[0-9]+)\\s+fps=\\s*(?<nfps>[0-9\\.]+)\\s+q=(?<nq>[0-9\\.-]+)\\s+(L?)\\s*size=\\s*(?<nsize>[0-9]+)(?<ssize>kB|mB|b)?\\s*time=\\s*(?<sduration>[0-9\\:\\.]+)\\s*bitrate=\\s*(?<nbitrate>[0-9\\.]+)(?<sbitrate>bits\\/s|mbits\\/s|kbits\\/s)?.*(dup=(?<ndup>\\d+)\\s*)?(drop=(?<ndrop>\\d+)\\s*)?speed=\\s*(?<nspeed>[0-9\\.]+)x");

    if (re.match(stderr).hasMatch()) {
        qInfo() << "FFmpeg status:" << stderr;
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
    qDebug() << "FFmpeg has finished with exit code" << exitCode << "and exit status" << exitStatus;
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
