#include "logger.h"
#include <iostream>

QFile* Logger::getLogFile()
{
    QString writableLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    if (writableLocation.isEmpty()) {
        QCoreApplication::exit(100);
    }

    if (!QDir().mkpath(writableLocation)) {
        QCoreApplication::exit(101);
    }

    QDir appData(writableLocation);

    QDate now = QDateTime::currentDateTimeUtc().date();
    QString fileName = QString::number(now.year()) + "_" + QString::number(now.month()) +
                       "_" + QString::number(now.day()) + "_fusion_ffmpeg_studio.log";

    QFile *logFile = new QFile(appData.path() + "/" + fileName);
    if (!logFile->open(QFile::ReadWrite | QFile::Append)) {
        QCoreApplication::exit(101);
    }
    logFile->write("FUSION FFMPEG STUDIO STARTING...\n");
    logFile->close();
    return logFile;
}

void Logger::debugHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";
    bool isLogOpen = true;
    QFile *logFile = Logger::getLogFile();
    for (int i=0; i<5; i++) {
        isLogOpen = logFile->isOpen();
    }
    if (isLogOpen) {
        std::cerr << "Debug handler could not open logFile, file in use" << std::endl;
        return;
    }
    if (!isLogOpen && !logFile->open(QFile::ReadWrite | QFile::Append)) {
        std::cerr << "Debug handler could not open logFile. Error number " << logFile->error() << " and error string " << logFile->errorString().toStdString() <<  std::endl;        return;
        return;
    }
    QString logLine;
    switch (type) {
    case QtDebugMsg:
        logLine.append("[DEBUG] ");
        break;
    case QtInfoMsg:
        logLine.append("[INFO] ");
        break;
    case QtWarningMsg:
        logLine.append("[WARNING] ");
        break;
    case QtCriticalMsg:
        logLine.append("[CRITICAL] ");
        break;
    case QtFatalMsg:
        logLine.append("[FATAL] ");
        break;
    }
    logLine.append(localMsg.constData()).append(file)
        .append(QString::number(context.line)).append(function);
    logFile->write(logLine.toUtf8());
    #ifdef QT_DEBUG
    if (type != 1000) std::cerr << logLine.toStdString() << std::endl;
    #endif
    logFile->close();
}
