#ifndef NEWPROJECTFOLDERCONTROLLER_H
#define NEWPROJECTFOLDERCONTROLLER_H

#include <QObject>

#include "controllers/basecontroller.h"

class NewProjectFolderController : public BaseController
{
    Q_OBJECT
public:
    explicit NewProjectFolderController(QObject *parent = nullptr);

    Q_INVOKABLE QString getDefaultProjectPath();
    Q_INVOKABLE bool verifyProjectPath(QString projectPath);

signals:
};

#endif // NEWPROJECTFOLDERCONTROLLER_H
