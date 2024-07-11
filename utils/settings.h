#ifndef SETTINGS_H
#define SETTINGS_H

#include <QCoreApplication>
#include <QString>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QProcess>
#include <iostream>


#include "utils/dialogs.h"
#include "utils/exitcodes.h"

class Settings
{
public:
    static void setup();
    static void setupAppData();
    static QString getAppDataPath();
private:
    static void setupProjectPath();
    static void setupBinaries();
    static void qexit(int code);

};

#endif // SETTINGS_H
