#include "welcomecontroller.h"

#include <QDebug>

WelcomeController::WelcomeController(QObject *parent)
    : BaseController{parent}
{
    qDebug() << "Created WelcomeController";
}

void WelcomeController::onLoadProjectClicked()
{
    m_appController->navigateToM(MainController::Pane::LoadProject);
}

void WelcomeController::onNewProjectClicked()
{
    m_appController->navigateToM(MainController::Pane::NewProject);
}

void WelcomeController::onAboutClicked()
{
    m_appController->addWindowM(MainController::Window::About, true);
}

void WelcomeController::onSettingsClicked()
{
    m_appController->addWindowM(MainController::Window::Preferences, true);
}
