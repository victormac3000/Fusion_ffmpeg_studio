#ifndef NEWPROJECTFOLDERCONTROLLER_H
#define NEWPROJECTFOLDERCONTROLLER_H

#include "controllers/basecontroller.h"

class NewProjectFolderController : public BaseController
{
    Q_OBJECT
public:
    explicit NewProjectFolderController(QObject *parent = nullptr);

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

#endif // NEWPROJECTFOLDERCONTROLLER_H
