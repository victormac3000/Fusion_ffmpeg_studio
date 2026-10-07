#ifndef LOCALDB_H
#define LOCALDB_H

#include "models/db/basedb.h"

class LocalDB: public BaseDB
{
public:
    LocalDB(QString dbPath);
    void updateRecentProjects(QString uuid, QString path, QString name, QDateTime savedOn);

};

#endif // LOCALDB_H
