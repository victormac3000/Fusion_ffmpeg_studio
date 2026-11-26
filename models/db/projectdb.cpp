#include "projectdb.h"
#include "utils/settings.h"

#include <QCoreApplication>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonObject>

ProjectDB::ProjectDB(QString dbPath):
    BaseDB(dbPath, ":/documents/json/projectdb.json")
{}

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
    QList<int> versions = Settings::getAppVersionNumbers();

    params.insert("uuid", uuid);
    params.insert("name", name);
    params.insert("dcim_path", dcimLinked ? dcimDir.absolutePath() : "");
    params.insert("dcim_linked", dcimLinked);
    params.insert("version_major", versions[0]);
    params.insert("version_mid", versions[1]);
    params.insert("version_minor", versions[2]);

    execute("UPDATE_PROJECT_INFO", params);
}

void ProjectDB::updateVideos(QList<FVideo> videos, QString projectDirPath)
{
    for (FVideo& video: videos) {
        execute("UPDATE_VIDEO", video.toVariantMap(projectDirPath));

        QList<FSegment> segments = video.getSegments();
        for (FSegment& segment: segments) {
            execute("UPDATE_SEGMENT", segment.toVariantMap(projectDirPath));
        }
    }
}

QVariantMap ProjectDB::getProjectInfo()
{
    QVariantList rows = query("GET_PROJECT_INFO");
    if (rows.length() > 0) {
        return rows.first().toMap();
    }
    return {};
}

QList<FVideo> ProjectDB::getVideos(QString projectDirPath)
{
    QList<FVideo> videos;

    QVariantList rows = query("GET_VIDEOS");

    for (QVariant& videoRow: rows) {
        FVideo video = FVideo::fromRow(videoRow, projectDirPath);

        QVariantList sRows = query("GET_SEGMENTS", {{"vid", video.getId()}});
        for (QVariant& segmentRow: sRows) {
            FSegment segment = FSegment::fromRow(video.getId(), segmentRow, projectDirPath);
            video.addSegment(segment);
        }

        videos.append(video);
    }

    return videos;
}
