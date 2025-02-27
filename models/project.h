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

    void load(LoadingInfo loadingInfo);
    void create(LoadingInfo loadingInfo);

    bool isValid();
    QDir getDcim();
    void setDcim(QString newDcim);
    QString getVersion();
    void setVersion(QString newVersion);
    QString getPath();
    void setVideos(QList<FVideo*> videos);
    QList<FVideo*> getVideos();
    QList<QPair<int,QString>> getBadVideos();

signals:
    void loadProjectUpdate(LoadingProgress progress);
    void loadProjectError(QString error);

public slots:
    void save();

private:
    bool valid;
    QDir dcim;
    bool dcimLinked = true;
    QString path;
    QString rootPath;
    QString version;
    QFile* file;
    QList<FVideo*> videos;
    QDateTime lastSaved;
    QDir front, back;
    LoadingProgress progress;
    QList<QPair<int,QString>> badVideos;

    void addToRecent();
    bool copyDCIM();
    bool indexVideos();
    void indexSegmentComplete();
    void indexVideoComplete();
    bool copy(QString src, QString dst);

};

#endif // PROJECT_H
