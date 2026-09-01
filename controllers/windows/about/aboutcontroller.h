#ifndef ABOUTCONTROLLER_H
#define ABOUTCONTROLLER_H

#include "controllers/basecontroller.h"

#include <QObject>

class AboutController : public BaseController
{
    Q_OBJECT
public:
    explicit AboutController(QObject *parent = nullptr);

    Q_INVOKABLE QString readResourceFile(const QString &path) const;

signals:
};

#endif // ABOUTCONTROLLER_H
