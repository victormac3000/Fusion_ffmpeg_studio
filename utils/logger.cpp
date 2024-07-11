#include "logger.h"

QHash<QtMsgType, QString> Logger::contextNames = {
    {QtMsgType::QtDebugMsg,		"DEBUG     "},
    {QtMsgType::QtInfoMsg,		"INFO      "},
    {QtMsgType::QtWarningMsg,	"WARNING   "},
    {QtMsgType::QtCriticalMsg,	"CRITICAL  "},
    {QtMsgType::QtFatalMsg,		"FATAL     "}
};
QFile* Logger::logFile = nullptr;
QDateTime Logger::logFileOpened;
QMutex Logger::logMutex;

void Logger::setup()
{
    Settings::setupAppData();
    logFile = getLogFile(true);
    qInstallMessageHandler(Logger::messageHandler);
}

QFile* Logger::getLogFile(bool init)
{
    QString appDataPath = Settings::getAppDataPath();
    QDir appData(appDataPath);

    if (!appData.exists("Logs") && !appData.mkdir("Logs")) {
        std::cerr << "Could not create the log folder inside appdata. The appData path is: "
                  << appData.absolutePath().toStdString()
                  << std::endl;
        if (init) qexit(ERROR_COULD_NOT_CREATE_LOG_FOLDER);
    }

    if (!appData.cd("Logs")) {
        std::cerr << "Could not read the log folder inside appdata. The appData path is: "
                  << appData.absolutePath().toStdString()
                  << std::endl;
        if (init) qexit(ERROR_COULD_NOT_READ_LOG_FOLDER);
    }

    logFileOpened = QDateTime::currentDateTime();
    QDate now = logFileOpened.date();
    QString fileName = QString::number(now.year()) + "_" + QString::number(now.month()) +
                       "_" + QString::number(now.day()) + "_fusion_ffmpeg_studio.log";

    QFile *logFile = new QFile(appData.absolutePath() + "/" + fileName);
    if (!logFile->open(QFile::ReadWrite | QFile::Append)) {
        std::cerr << "Could not open log in getLogFile. The logfile path is: "
                  << logFile->fileName().toStdString()
                  << std::endl;
        if(init) qexit(ERROR_COULD_NOT_OPEN_LOG);
    }

    logFile->write("\n");

    if (logFile->size() < 1) {
        logFile->write("\n");
    }

    logFile->close();
    return logFile;
}

void Logger::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (QDateTime::currentDateTime().date().day() != logFileOpened.date().day()) {
        logFile = getLogFile();
    }

    QString logLine = QString("%1 | %2 | %3 | %4 | %5 | %6\n").arg(
        QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"),
        Logger::contextNames.value(type),
        QString::number(context.line),
        QString(context.file),
        QString(context.function),
        msg
    );

    if (logFile == nullptr) {
        std::cerr << "The logFile is a nullptr" << std::endl;
        return;
    }

    bool debug = false;

    #ifdef QT_DEBUG
        debug = true;
    #endif

    debug = debug || QCoreApplication::arguments().contains("-v");

    if (debug) {
        std::cout << logLine.toStdString() << std::endl;
    }

    QMutexLocker locker(&logMutex);

    if (!logFile->open(QFile::ReadWrite | QFile::Append)) {
        std::cerr << "Debug handler could not open logFile. Error number " << logFile->error()
                  << " and error string " << logFile->errorString().toStdString()
                  <<  ". The log file trying to open is: " << logFile->fileName().toStdString() << std::endl;
        return;
    }

    logFile->write(logLine.toUtf8());

    logFile->close();
}

void Logger::qexit(int code)
{
    QCoreApplication::exit(code);
    exit(code);
}
