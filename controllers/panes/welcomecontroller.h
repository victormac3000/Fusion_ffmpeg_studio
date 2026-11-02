#ifndef WELCOMECONTROLLER_H
#define WELCOMECONTROLLER_H

#include "controllers/basecontroller.h"

#include <QObject>
#include <QQmlEngine>

class WelcomeController : public BaseController
{
    Q_OBJECT
public:
    explicit WelcomeController(QObject *parent = nullptr);

    Q_INVOKABLE void loadRecentProject(const QJSValue& args,
                                       const QJSValue& outputCallback,
                                       const QJSValue& errorCallback);
    Q_INVOKABLE void getRecentProjects(const QJSValue& args,
                                       const QJSValue& outputCallback,
                                       const QJSValue& errorCallback);
    Q_INVOKABLE void onLoadProjectClicked();
    Q_INVOKABLE void onNewProjectClicked();
    Q_INVOKABLE void onAboutClicked();
    Q_INVOKABLE void onSettingsClicked();

signals:
};

#endif // WELCOMECONTROLLER_H
