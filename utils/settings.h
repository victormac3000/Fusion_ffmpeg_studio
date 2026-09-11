#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QProcess>
#include <QRegularExpression>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class Settings
{
public:
    static void setup();

    static QStringList getAvailableCodecs();
    static QStringList getAvailableEncoders(QString codec);
    static QStringList getAvailableFormats(QString codec);
    static QString getAppDataPath();
    static QString getDefaultAppDataPath();
    static QString getDefaultProjectName();
    static QString getDefaultProjectPath();
    static QString getFFmpegPath();
    static QString getFFprobePath();
    static QString getDefaultCodec();
    static QString getDefaultEncoder();
    static QString getDefaultFormat();

    static void setDefaultCodec(QString defaultCodec);
    static void setDefaultEncoder(QString defaultEncoder);
    static void setDefaultFormat(QString defaultFormat);

    static void setAppDataPath(const QString& newPath);
    static void setDefaultProjectName(QString projectName);
    static void setDefaultProjectPath(QString projectPath);

    static void resetDefaultFormat();
    static void resetAppDataPath();
    static void resetDefaultProjectPath();

private:
    static void setupAppData();
    static void setupDefaultProjectPath();
    static void setupDefaultProjectName();
    static void setupBinaries();
    static void setupEncoders();
    static void setupLocalDb();

    static void qexit(int code);

    static QMap<QString, QStringList> compatibleFormats;

};

#endif // SETTINGS_H
