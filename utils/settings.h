#ifndef SETTINGS_H
#define SETTINGS_H

#include <QCoreApplication>
#include <QString>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QProcess>
#include <iostream>
#include <QRegularExpression>
#include <QMap>

#include "utils/dialogs.h"
#include "utils/exitcodes.h"
#include "utils/myqsysinfo.h"

struct FormatInfo {
    QStringList supportedFormats;
    QString description;
};

class Settings
{
public:
    static void setup();

    static QString getAppDataPath();
    static QString getDefaultProjectName();
    static QString getDefaultProjectPath();
    static QString getFFmpegPath();
    static QString getFFprobePath();

    static QString getDefaultCodec();
    static QString getDefaultEncoder();
    static void resetDefaultFormat();
    static QString getDefaultFormat();

    static void setDefaultCodec(QString defaultCodec);
    static void setDefaultEncoder(QString defaultEncoder);
    static void setDefaultFormat(QString defaultFormat);

    static QStringList getAvailableCodecs();
    static QStringList getAvailableEncoders(QString codec);
    static QStringList getAvailableFormats(QString codec);

private:
    static void setupAppData();
    static void setupDefaultProjectPath();
    static void setupDefaultProjectName();
    static void setupBinaries();
    static void setupEncoders();

    static void qexit(int code);

    static QMap<QString, FormatInfo> compatibleFormats;

};

#endif // SETTINGS_H
