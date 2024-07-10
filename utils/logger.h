#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QFileInfo>
#include <QThread>

#include "utils/settings.h"
#include "utils/exitcodes.h"

class Logger
{
public:
    static void setup();
    static QFile* getLogFile();
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
private:
    static QHash<QtMsgType, QString> contextNames;
    static QFile* logFile;
};

#endif // LOGGER_H
