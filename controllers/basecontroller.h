#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include "controllers/maincontroller.h"

#include <QObject>
#include <QJSValue>
#include <QVariantMap>

class BaseController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(MainController* appController READ appController WRITE setAppController
                                             NOTIFY appControllerChanged REQUIRED)

public:
    explicit BaseController(QObject *parent = nullptr);

    MainController *appController() const;
    void setAppController(MainController *appController);

protected:
    using BackgroundTaskFunction =
        std::function<QVariantMap(const QVariantMap &args)>;

    using BackgroundTaskDoneFunction =
        std::function<void(
            const QVariantMap &result,
            const QString &error,
            const QJSValue &outputCallback,
            const QJSValue &errorCallback)>;

    using TaskDoneFunction =
        std::function<void(
            const QVariantMap &result,
            const QJSValue &outputCallback)>;

    void backgroundTask(
        const QJSValue &args,
        const QJSValue &outputCallback,
        const QJSValue &errorCallback,
        BackgroundTaskFunction workFunction,
        BackgroundTaskDoneFunction doneFunction
    );

    void task(
        const QJSValue &args,
        const QJSValue &outputCallback,
        const QJSValue &errorCallback,
        BackgroundTaskFunction workFunction,
        TaskDoneFunction doneFunction
    );

    MainController *m_appController = nullptr;

signals:
    void appControllerChanged();
};

#endif // BASECONTROLLER_H