#ifndef PROJECT_H
#define PROJECT_H

class FVideo;
struct LoadingInfo;

#include "loading.h"

#include <QObject>
#include <QList>
#include <QCoreApplication>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonObject>
#include <QSettings>
#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class Project : public QObject
{
    Q_OBJECT
public:
    explicit Project(QObject *parent = nullptr);
    ~Project();

    bool isValid();
    QStringList getErrors();
    QStringList getWarnings();
    void moveToNewThread(QThread* newThread);
    void load(LoadingInfo loadingInfo);
    void create(LoadingInfo loadingInfo);
    QDir getDcim();
    void setDcim(QString newDcim);
    QString getVersion();
    void setVersion(QString newVersion);
    QString getPath();
    void setVideos(QList<FVideo*> videos);
    QList<FVideo*> getVideos();
    QList<QPair<QString,QString>> getBadVideos();
    bool save();

signals:
    void loadProjectUpdate(LoadingProgress progress);


private:
    bool valid = false;

    QString uuid;
    QString path;
    QString rootPath;
    QDir dcim;
    bool dcimLinked = true;
    QDir front, back;
    QString version;

    QList<FVideo*> videos;
    QList<QPair<QString,QString>> badVideos;
    LoadingProgress progress;
    QDateTime lastSaved;

    void addToRecent();
    bool setupDatabase();
    bool copyDCIM();
    bool indexVideos();
    void indexSegmentComplete();
    void indexVideoComplete();
    bool copy(QString src, QString dst);
    QList<int> getVersionNumbers();
    QStringList errors;
    QStringList warnings;

};

#endif // PROJECT_H
