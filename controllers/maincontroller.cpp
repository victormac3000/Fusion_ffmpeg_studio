#include "maincontroller.h"

#include <QDebug>
#include <QMetaEnum>
#include <QGuiApplication>

MainController::MainController(QObject *parent)
    : QObject{parent}
{

}

QString MainController::paneToUrl(Pane pane) const
{
    QString baseUrl = "qrc:/qt/qml/FusionFFmpegStudio/qml/panes";

    switch (pane)
    {
        case Pane::Welcome:
            return baseUrl + "/welcome/Welcome.qml";

        case Pane::LoadProject:
            return baseUrl + "/loadproject/LoadProject.qml";

        case Pane::NewProject:
            return baseUrl + "/new-project/NewProject.qml";

        case Pane::NewProjectFolder:
            return baseUrl + "/new-project/NewProjectFolder.qml";

        case Pane::NewProjectSd:
            return baseUrl + "/new-project/NewProjectSd.qml";
    }

    return "";
}

QString MainController::windowToUrl(Window window) const
{
    QString baseUrl = "qrc:/qt/qml/FusionFFmpegStudio/qml/windows";

    switch (window)
    {
        case Window::Main:
            return baseUrl + "/main/Main.qml";

        case Window::About:
            return baseUrl + "/about/About.qml";

        case Window::Preferences:
            return baseUrl + "/preferences/Preferences.qml";
    }

    return "";

}

void MainController::stopApp()
{
    QGuiApplication::exit(1);
}

void MainController::navigateToM(Pane pane)
{
    emit navigateTo(pane);
}

void MainController::addWindowM(Window window, bool modal)
{
    emit addWindow(window, modal);
}

void MainController::backM()
{

}
