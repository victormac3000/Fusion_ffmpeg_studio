#include "newprojectcontroller.h"

#include <QDebug>

NewProjectController::NewProjectController(QObject *parent)
    : BaseController{parent}
{}

void NewProjectController::importSDClicked()
{
    m_appController->navigateToM(MainController::Pane::NewProjectSd);
}

void NewProjectController::importDCIMFolderClicked()
{
    m_appController->navigateToM(MainController::Pane::NewProjectFolder);
}
