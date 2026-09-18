#include "maincontroller.h"

#include <QDebug>
#include <QMetaEnum>
#include <QGuiApplication>

MainController::MainController(QObject *parent)
    : QObject{parent}
{
    this->project = new Project();
    this->project->moveToThread(&projectThread);
    projectThread.start();
}

MainController::~MainController()
{
    projectThread.quit();
    projectThread.wait();
    delete project;
}

QString MainController::paneToUrl(Pane pane) const
{
    QString baseUrl = "qrc:/qt/qml/FusionFFmpegStudio/qml/panes";

    switch (pane)
    {
        case Pane::Welcome:
            return baseUrl + "/Welcome.qml";

        case Pane::NewProject:
            return baseUrl + "/NewProject.qml";

        case Pane::Loading:
            return baseUrl + "/Loading.qml";
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

void MainController::navigateToM(Pane pane,  const QVariantMap &attributes)
{
    emit navigateTo(pane, attributes);
}

void MainController::addWindowM(Window window, bool modal)
{
    emit addWindow(window, modal);
}

void MainController::backM()
{

}

Project* MainController::getProject()
{
    return this->project;
}
