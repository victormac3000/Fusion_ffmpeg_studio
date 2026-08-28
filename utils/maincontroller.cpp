#include "maincontroller.h"

#include <QDebug>

MainController::MainController(QObject *parent)
    : QObject{parent}
{

}

void MainController::onLoadProjectClicked()
{
    emit navigateTo(Pane::LoadProject);
}

void MainController::onNewProjectClicked()
{
    emit navigateTo(Pane::NewProject);
}

void MainController::onAboutClicked()
{
    emit navigateTo(Pane::About);
}

void MainController::onSettingsClicked()
{
    emit navigateTo(Pane::Settings);
}
