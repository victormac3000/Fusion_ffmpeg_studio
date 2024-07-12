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


#include "utils/dialogs.h"
#include "utils/exitcodes.h"

class Settings
{
public:
    static void setup();

    static QString getAppDataPath();
    static QString getDefaultProjectName();
    static QString getDefaultProjectPath();
    static QString getFFmpegPath();
    static QString getFFprobePath();
    static QStringList getAvailableEncoders();
private:
    static void setupAppData();
    static void setupDefaultProjectPath();
    static void setupDefaultProjectName();
    static void setupBinaries();
    static void setupBinariesWin(QString* ffmpegPath, QString* ffprobePath);
    static void setupBinariesLin(QString* ffmpegPath, QString* ffprobePath);
    static void setupBinariesMac(QString* ffmpegPath, QString* ffprobePath);
    static void setupEncoders();
    static void qexit(int code);

};

#endif // SETTINGS_H
