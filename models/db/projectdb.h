#ifndef PROJECTDB_H
#define PROJECTDB_H

#include <QSqlDatabase>
#include <QDir>

#include "models/fvideo.h"
#include "models/db/basedb.h"

class ProjectDB: public BaseDB
{
public:
    ProjectDB(QString dbPath, QString uuid,
              QString name, QDir dcimDir,
              bool dcimLinked);
    void updateVideos(QList<FVideo> videos);
    void addProjectToRecent();

private:
    QList<int> getVersionNumbers();
};

#endif // PROJECTDB_H
