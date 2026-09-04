#include "aboutcontroller.h"
#include "utils/ffmpeg.h"

#include <QFile>
#include <QDebug>
#include <QVariant>

AboutController::AboutController(QObject *parent)
    : BaseController{parent}
{

}

QString AboutController::readResourceFile(const QString &path) const
{
    QFile file(path);

    if (!file.open(QFile::ReadOnly)) {
        qWarning() << "Could not open file:" << path;
        return "";
    }

    return QString::fromUtf8(file.readAll());
}

QVariantMap AboutController::getFFmpegVersions() const
{
    FFmpeg ffmpeg;
    QMap<QString,QString> versionsMap = ffmpeg.getVersions();

    QVariantMap versionsMapConverted;
    QMapIterator<QString, QString> i(versionsMap);
    while (i.hasNext()) {
        i.next();
        versionsMapConverted.insert(i.key(), i.value());
    }

    return versionsMapConverted;
}

QString AboutController::getQtVersion() const
{
    return QString::fromUtf8(qVersion());
}

QVariantMap AboutController::getBuildInfo() const
{
    QString buildDatetime = QStringLiteral(__DATE__) + " " + QStringLiteral(__TIME__);

    QString compilationType = "Release";
    #ifdef QT_DEBUG
        compilationType = "Debug";
    #endif

    return QVariantMap {
        {"datetime", buildDatetime},
        {"type", compilationType},
        {"os", QSysInfo::productType()},
        {"os_version", QSysInfo::productVersion()}
    };
}
