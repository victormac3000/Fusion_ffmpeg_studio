#include "ffmpeg.h"

FFmpeg::FFmpeg(QObject *parent)
    : QObject{parent}
{
    QString ffmpegPath = getPath();
    qDebug() << "FFMpeg binary path found:" << ffmpegPath;
    this->process = new QProcess(this);

    connect(process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(processErrorOccurred(QProcess::ProcessError)));
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int,QProcess::ExitStatus)));
    connect(process, SIGNAL(readyReadStandardError()), this, SLOT(processReadyReadError()));
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(processReadyReadOut()));
    connect(process, SIGNAL(started()), this, SLOT(processStarted()));
    connect(process, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(processStateChanged(QProcess::ProcessState)));

    this->process->setProgram(ffmpegPath);
    this->params.append("-hide_banner");
    this->params.append("-loglevel");
    this->params.append("error");
    this->params.append("-stats");

    qDebug() << "FFMpeg created";
}

FFmpeg::~FFmpeg()
{
    qDebug() << "FFMpeg destroyed";
}

void FFmpeg::preRender(FVideo *video)
{
    params.append("-i");
    params.append("'" + video->getSegment(0)->getFrontMP4()->fileName() + "'");
    params.append("-i");
    params.append("'" + video->getSegment(0)->getBackMP4()->fileName() + "'");
    params.append("-filter_complex");
    params.append("hstack");
    params.append("-c:v");
    params.append("libx265");
    params.append("'" + settings.value("renderedDir").toString() + "/VIDEO_" + video->getIdString() + ".MP4'");
    QString cli;
    for (const QString &a: params) {
        cli.append(a+" ");
    }
    qDebug() << "Ffmpeg command:" << cli;
    process->setArguments(params);
    process->startDetached();
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
    qWarning() << "FFmpeg has given an stdout error:" << process->readAllStandardError();
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
