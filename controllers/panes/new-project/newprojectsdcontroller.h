#ifndef NEWPROJECTSDCONTROLLER_H
#define NEWPROJECTSDCONTROLLER_H

#include "controllers/basecontroller.h"

class NewProjectSdController : public BaseController
{
    Q_OBJECT
public:
    explicit NewProjectSdController(QObject *parent = nullptr);

    Q_INVOKABLE QString getDefaultProjectName();
    Q_INVOKABLE QString getDefaultProjectPath();
    Q_INVOKABLE void generateProjectPath(const QJSValue& args,
                                         const QJSValue& outputCallback,
                                         const QJSValue& errorCallback);
    Q_INVOKABLE void validateProjectName(const QJSValue& args,
                                         const QJSValue& outputCallback,
                                         const QJSValue& errorCallback);
    Q_INVOKABLE void validateProjectPath(const QJSValue& args,
                                         const QJSValue& outputCallback,
                                         const QJSValue& errorCallback);

private:
    QPair<QString,QString> generateProjectNamePath(QString name, QString basePath);

signals:
};

#endif // NEWPROJECTSDCONTROLLER_H
