#ifndef LOGGER_H
#define LOGGER_H

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QFileInfo>
#include <QThread>
#include <QMutex>

class Logger
{
public:
    static void setup();
    static QFile* getLogFile(bool init = false);
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
private:
    static QHash<QtMsgType, QString> contextNames;
    static void qexit(int code);

    static QFile* logFile;
    static QDateTime logFileOpened;
    static QMutex logMutex;
};

#endif // LOGGER_H
