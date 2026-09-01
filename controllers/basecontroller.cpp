#include "basecontroller.h"

BaseController::BaseController(QObject *parent)
    : QObject(parent)
{
}

MainController *BaseController::appController() const
{
    return m_appController;
}

void BaseController::setAppController(MainController *appController)
{
    if (m_appController == appController)
        return;

    m_appController = appController;

    emit appControllerChanged();
}