#ifndef WELCOMECONTROLLER_H
#define WELCOMECONTROLLER_H

#include "controllers/basecontroller.h"

#include <QObject>

class WelcomeController : public BaseController
{
    Q_OBJECT
public:
    explicit WelcomeController(QObject *parent = nullptr);

    Q_INVOKABLE void onLoadProjectClicked();
    Q_INVOKABLE void onNewProjectClicked();
    Q_INVOKABLE void onAboutClicked();
    Q_INVOKABLE void onSettingsClicked();

signals:
};

#endif // WELCOMECONTROLLER_H
