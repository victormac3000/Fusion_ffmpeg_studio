#ifndef NEWPROJECTCONTROLLER_H
#define NEWPROJECTCONTROLLER_H

#include "controllers/basecontroller.h"

#include <QObject>


class NewProjectController : public BaseController
{
    Q_OBJECT
public:
    explicit NewProjectController(QObject *parent = nullptr);

    Q_INVOKABLE QString getDefaultProjectName();
    Q_INVOKABLE QString getDefaultProjectPath();
    Q_INVOKABLE QVariantList getExternalVolumes();

    Q_INVOKABLE void validateProjectName(const QJSValue& args,
                                         const QJSValue& outputCallback,
                                         const QJSValue& errorCallback);
    Q_INVOKABLE void generateProjectPath(const QJSValue& args,
                                         const QJSValue& outputCallback,
                                         const QJSValue& errorCallback);
    Q_INVOKABLE void validateProjectpath(const QJSValue& args,
                                         const QJSValue& outputCallback,
                                         const QJSValue& errorCallback);
    Q_INVOKABLE void validateDCIMPath(const QJSValue& args,
                                      const QJSValue& outputCallback,
                                      const QJSValue& errorCallback);
    Q_INVOKABLE void createProject(const QJSValue& args,
                                   const QJSValue& outputCallback,
                                   const QJSValue& errorCallback);

private:
    QPair<QString,QString> generateProjectNamePath(QString name, QString basePath);

signals:
};

#endif // NEWPROJECTCONTROLLER_H
