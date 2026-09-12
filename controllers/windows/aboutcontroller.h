#ifndef ABOUTCONTROLLER_H
#define ABOUTCONTROLLER_H

#include "controllers/basecontroller.h"

#include <QObject>
#include <QMap>
#include <QVariantMap>

class AboutController : public BaseController
{
    Q_OBJECT
public:
    explicit AboutController(QObject *parent = nullptr);

    Q_INVOKABLE QString readResourceFile(const QString &path) const;
    Q_INVOKABLE QVariantMap getFFmpegVersions() const;
    Q_INVOKABLE QString getQtVersion() const;
    Q_INVOKABLE QVariantMap getBuildInfo() const;

signals:
};

#endif // ABOUTCONTROLLER_H
