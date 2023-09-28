#include "logger.h"
#include <iostream>

QHash<QtMsgType, QString> Logger::contextNames = {
    {QtMsgType::QtDebugMsg,		"   DEBUG  "},
    {QtMsgType::QtInfoMsg,		"   INFO   "},
    {QtMsgType::QtWarningMsg,	"  WARNING "},
    {QtMsgType::QtCriticalMsg,	" CRITICAL "},
    {QtMsgType::QtFatalMsg,		" FATAL  "}
};

QFile* Logger::getLogFile()
{
    QString appDataPath = Settings::getAppDataPath();


    QDate now = QDateTime::currentDateTime().date();
    QString fileName = QString::number(now.year()) + "_" + QString::number(now.month()) +
                       "_" + QString::number(now.day()) + "_fusion_ffmpeg_studio.log";

    QFile *logFile = new QFile(appDataPath + "/" + fileName);
    if (!logFile->open(QFile::ReadWrite | QFile::Append)) {
        std::cerr << "Could not open log in getLogFile. The logfile path is: " << logFile->fileName().toStdString() << std::endl;
        exit(ERROR_COULD_NOT_OPEN_LOG);
    }
    logFile->write("FUSION FFMPEG STUDIO STARTING...\n");
    logFile->close();
    return logFile;
}

void Logger::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    bool isLogOpen = true;
    QFile *logFile = Logger::getLogFile();

    for (int i=0; i<5; i++) {
        isLogOpen = logFile->isOpen();
        QThread::msleep(100);
    }

    if (isLogOpen) {
        std::cerr << "Debug handler could not open logFile, file in use. The log file is: " << logFile->fileName().toStdString() << std::endl;
        exit(ERROR_LOG_IN_USE);
    }

    if (!isLogOpen && !logFile->open(QFile::ReadWrite | QFile::Append)) {
        std::cerr << "Debug handler could not open logFile. Error number " << logFile->error()
                  << " and error string " << logFile->errorString().toStdString()
                  <<  ". The log file trying to open is: " << logFile->fileName().toStdString() << std::endl;
        exit(ERROR_COULD_NOT_OPEN_LOG);
    }

    QString logLine = QString("%1 | %2 | %3 | %4 | %5 | %6\n").
                    arg(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss")).
                    arg(Logger::contextNames.value(type)).
                    arg(context.line).
                    arg(QString(context.file).
                        section('\\', -1)).
                    arg(QString(context.function).
                        section('(', -2, -2).
                        section(' ', -1).
                        section(':', -1)).
                    arg(msg);

    logFile->write(logLine.toUtf8());

    #ifdef QT_DEBUG
    std::cout << logLine.toStdString() << std::endl;
    #endif

    logFile->close();
    delete logFile;
}
