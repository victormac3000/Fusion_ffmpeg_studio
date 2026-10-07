#ifndef PROJECT_H
#define PROJECT_H

#include "loading.h"
#include "models/db/projectdb.h"
#include "models/db/localdb.h"
#include "models/fvideo.h"
#include "models/loading.h"

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

    void save();

signals:
    void loadProjectUpdate(LoadingProgress progress);
    void loadProjectDone(LoadingDone done);
    void loadProjectError(LoadingError error);

protected slots:
    void create(LoadingInfo info);

private:
    // Project attributes

    QString uuid;
    QString name;
    QDir rootDir;
    QDir dir;
    QDir dcim;
    bool dcimLinked = false;
    QDir frontDir;
    QDir backDir;


    // Other
    ProjectDB* dbManager;
    LocalDB* localDBManager;
    QList<FVideo> videos;
    QMap<QString,QString> badVideos;

    LoadingInfo loadingInfo;
    LoadingProgress loadingProgress;

    QDateTime lastSaved;

    void createProjectFolder();
    void createProjectSD();
    void loadProject();

    void createProjectTGenerate();
    void createProjectTCopyDCIM();
    bool createProjectTHCopy(QString src, QString dst);

    void indexVideos();
    void addToRecent();

    void indexSegmentComplete();
    void indexVideoComplete();

};

#endif // PROJECT_H
