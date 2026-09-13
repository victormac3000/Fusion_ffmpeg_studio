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
    Q_INVOKABLE void validateProjectPath(const QJSValue& args,
                                    const QJSValue& outputCallback,
                                    const QJSValue& errorCallback);
    Q_INVOKABLE void validateDCIMPath(const QJSValue& args,
                                    const QJSValue& outputCallback,
                                    const QJSValue& errorCallback);

signals:
};

#endif // NEWPROJECTFOLDERCONTROLLER_H
