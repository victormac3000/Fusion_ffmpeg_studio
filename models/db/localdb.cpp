#include "localdb.h"

#include <QDateTime>

LocalDB::LocalDB(QString dbPath):
    BaseDB(dbPath, ":/documents/json/local.json")
{
    execute("CREATE_RECENT_PROJECTS");
}

void LocalDB::updateRecentProjects(QString uuid, QString path, QString name, QDateTime savedOn)
{
    QVariantMap params;

    params.insert("uuid", uuid);
    params.insert("path", path);
    params.insert("name", name);
    params.insert("saved_on", savedOn.toMSecsSinceEpoch());

    execute("UPDATE_RECENT_PROJECTS", params);
}
