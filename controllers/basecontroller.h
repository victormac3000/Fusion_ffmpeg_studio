#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include "controllers/maincontroller.h"

#include <QObject>

class BaseController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(MainController* appController
                   READ appController
                       WRITE setAppController
                           NOTIFY appControllerChanged
                               REQUIRED)

public:
    explicit BaseController(QObject *parent = nullptr);

    MainController *appController() const;
    void setAppController(MainController *appController);

protected:
    MainController *m_appController = nullptr;

signals:
    void appControllerChanged();
};

#endif // BASECONTROLLER_H