#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QStandardPaths>
#include <QFileInfo>
#include <QSettings>

class Logger
{
public:
    static QFile* getLogFile();
    static void debugHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
private:
    static QHash<QtMsgType, QString> contextNames;
};

#endif // LOGGER_H
