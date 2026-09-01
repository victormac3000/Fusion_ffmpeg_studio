#include "newprojectcontroller.h"

#include <QDebug>

NewProjectController::NewProjectController(QObject *parent)
    : BaseController{parent}
{
    qDebug() << "Created NewProjectController";
}

void NewProjectController::importSDClicked()
{
    m_appController->navigateToM(MainController::Pane::NewProjectSd);
}

void NewProjectController::importDCIMFolderClicked()
{
    m_appController->navigateToM(MainController::Pane::NewProjectFolder);
}

void NewProjectController::onBackClicked()
{
    m_appController->back();
}
