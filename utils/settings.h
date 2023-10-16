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
    static QString getAppDataPath();
private:
    static void checkCompatible();
    static void setupAppData();
    static void setupProjectPath();
    static void setupBinaries();

};

#endif // SETTINGS_H
