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
    explicit Project(QObject *parent, QString projectFilePath);
    ~Project();

    bool isValid();
    QString getDcim();
    void setDcim(QString newDcim);
    QString getVersion();
    void setVersion(QString newVersion);
    QString getPath();
    void setPath(QString newPath);
    QFile *getFile();
    void setFile(QFile *newFile);
    void setVideos(QList<FVideo*> videos);
    QList<FVideo*> getVideos();


public slots:
    void save();

private:
    bool valid;
    QJsonDocument json;
    QString dcim;
    QString version;
    QString path;
    QFile *file;
    QList<FVideo*> videos;
    QDateTime lastSaved;


};

#endif // PROJECT_H
