#ifndef LOCALDB_H
#define LOCALDB_H

#include "models/db/basedb.h"

#include <QDateTime>

struct RecentProject {
    QString uuid;
    QString path;
    QString name;
    QDateTime savedOn;
};

class LocalDB: public BaseDB
{
public:
    LocalDB(QString dbPath = "");
    void updateRecentProjects(QString uuid, QString path, QString name, QDateTime savedOn);
    QList<RecentProject> getRecentProjects();

};

#endif // LOCALDB_H
