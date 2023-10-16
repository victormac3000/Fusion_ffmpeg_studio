#ifndef PROJECT_H
#define PROJECT_H

#include "fvideo.h"

#include <QObject>
#include <QList>
#include <QDomDocument>
#include <QCoreApplication>

class Project : public QObject
{
    Q_OBJECT
public:
    explicit Project(QObject *parent = nullptr, bool create = false, QString projectPath = "", QString dcimPath = "", QString projectName = "");
    ~Project();

    bool isValid();
    QDir getDcim();
    void setDcim(QString newDcim);
    QString getVersion();
    void setVersion(QString newVersion);
    QString getPath();
    void setVideos(QList<FVideo*> videos);
    QList<FVideo*> getVideos();

signals:
    void loadProjectUpdate(int percent = 0, QString message = "");
    void loadProjectError(QString error);

public slots:
    void save();

private:
    bool valid;
    QDir dcim;
    QString path;
    QString name;
    QString version;
    QFile *file;
    QList<FVideo*> videos;
    QDateTime lastSaved;

    void loadProject(QString projectPath);
    void createProject(QString projectPath, QString dcimPath, QString projectName);
    bool indexVideos();
    void indexSegmentComplete(int doneSegments, int totalSegments);
    void indexVideoComplete(int doneVideos, int totalVideos);


};

#endif // PROJECT_H
