#ifndef LOADINGCONTROLLER_H
#define LOADINGCONTROLLER_H

#include "controllers/basecontroller.h"

class LoadingController : public BaseController
{
    Q_OBJECT
public:
    explicit LoadingController(QObject *parent = nullptr);

    Q_INVOKABLE void startLoading(const QJSValue& args,
                                  const QJSValue& outputCallback,
                                  const QJSValue& errorCallback);

private slots:
    void onLoadProjectUpdate(LoadingProgress progressData);
    void onLoadProjectDone(LoadingDone done);
    void onLoadProjectError(LoadingError error);

signals:
    void loadProjectError(const QVariantMap& errorData);
    void loadProjectDone(const QVariantMap& doneData);
    void loadProjectUpdate(const QVariantMap& updateData);

private:
    Project* project;
};

#endif // LOADINGCONTROLLER_H
