#include "projectdb.h"

#include <QCoreApplication>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonObject>

ProjectDB::ProjectDB(QString dbPath, QString uuid,
                     QString name, QDir dcimDir,
                     bool dcimLinked):
    BaseDB(dbPath, ":/documents/json/projectdb.json")
{
    // Create tables
    execute("CREATE_PROJECT_INFO");
    execute("CREATE_VIDEOS");
    execute("CREATE_SEGMENTS");

    // Insert project data
    QVariantMap params;
    QList<int> versions = getVersionNumbers();

    params.insert("uuid", uuid);
    params.insert("name", name);
    params.insert("dcim_path", dcimLinked ? dcimDir.absolutePath() : "");
    params.insert("dcim_linked", dcimLinked);
    params.insert("version_major", versions[0]);
    params.insert("version_mid", versions[1]);
    params.insert("version_minor", versions[2]);

    execute("UPDATE_PROJECT_INFO", params);
}

void ProjectDB::updateVideos(QList<FVideo> videos)
{
    for (FVideo& video: videos) {
        QVariantMap params;

        params.insert("id", video.getId());
        params.insert("dualFisheye", false);
        params.insert("dualFisheyeLow", false);
        params.insert("equirectangular", false);
        params.insert("equirectangularLow", false);

        execute("UPDATE_VIDEO", params);

        QList<FSegment> segments = video.getSegments();
        for (FSegment& segment: segments) {
            params = QVariantMap{};

            params.insert("id", segment.getId());
            params.insert("video", video.getId());
            params.insert("dualFisheye", false);
            params.insert("dualFisheyeLow", false);

            execute("UPDATE_SEGMENT", params);
        }
    }
}

QList<int> ProjectDB::getVersionNumbers()
{
    QString version = QCoreApplication::applicationVersion();
    QStringList versionList = version.split(".");
    return {
        (versionList.length()>0) ? versionList.at(0).toInt() : 0,
        (versionList.length()>1) ? versionList.at(1).toInt() : 0,
        (versionList.length()>2) ? versionList.at(2).toInt() : 0
    };
}
