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
#include <QCryptographicHash>


#ifdef Q_OS_MAC
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

#ifdef Q_OS_WIN

#endif

#ifdef Q_OS_LIN

#endif

#include "utils/dialogs.h"
#include "utils/exitcodes.h"

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
    static QString getDefaultFormat();
    static QStringList getAvailableCodecs();
    static QStringList getAvailableEncoders();
    static QStringList getAvailableFormats(QString codec);

private:
    static void setupAppData();
    static void setupDefaultProjectPath();
    static void setupDefaultProjectName();
    static void setupBinaries();
    static void setupBinariesWin(QString* ffmpegPath, QString* ffprobePath);
    static void setupBinariesLin(QString* ffmpegPath, QString* ffprobePath);
    static void setupBinariesMac(QString* ffmpegPath, QString* ffprobePath);
    static void setupEncoders();
    static QByteArray getHardwareId();
    static QStringList getGpuNames();
    static QString getCpuName();
    static QString getMotherboardName();


    static void qexit(int code);

    static QMap<QString, FormatInfo> compatibleFormats;

};

#endif // SETTINGS_H
