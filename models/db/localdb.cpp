#include "localdb.h"
#include "utils/settings.h"

#include <QDateTime>

LocalDB::LocalDB(QString dbPath):
    BaseDB(dbPath.isEmpty() ? Settings::getLocalDBPath() : dbPath, ":/documents/json/local.json")
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

QList<RecentProject> LocalDB::getRecentProjects()
{
    QList<RecentProject> recentProjects;

    QVariantList result = query("GET_RECENT_PROJECTS");
    for (QVariant& rawRow: result) {
        QVariantMap row = rawRow.toMap();
        recentProjects.append({
            .uuid = row.value("uuid").toString(),
            .path = row.value("path").toString(),
            .name = row.value("name").toString(),
            .savedOn = QDateTime::fromMSecsSinceEpoch(row.value("saved_on").toInt())
        });
    }

    return recentProjects;
}
