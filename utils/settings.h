#ifndef SETTINGS_H
#define SETTINGS_H

#include <QCoreApplication>
#include <QString>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QProcess>


#include "utils/dialogs.h"

class Settings
{
public:
    static void setup();
    static QString getAppDataPath();
private:
    static void checkCompatible();
};

#endif // SETTINGS_H
