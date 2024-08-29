#include "projectcreator.h"
#include "ui_projectcreator.h"
#include "windows/mainwindow.h"
#include "panes/welcomepane.h"
#include "panes/projectcreatorfolder.h"
#include "panes/projectcreatorsd.h"

ProjectCreator::ProjectCreator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectCreator)
{
    ui->setupUi(this);

    this->mainWindow = (MainWindow*) parent;

    if (mainWindow != nullptr) {
        mainWindow->setTitle("Create project");
    } else {
        Dialogs::critical(
            "Error loading the project creator",
            "MainWindow pointer is null"
        );
        return;
    }

    QQuickItem* root = ui->quickWidget->rootObject();

    if (root == nullptr) {
        Dialogs::critical(
            "Error loading the project creator",
            "Could not found quickWidget element root QML object. Rolling back to Welcome Pane"
        );
        return;
    }

    QQuickItem* sdCardMouseArea = root->findChild<QQuickItem*>("sdCardMouseArea");
    QQuickItem* dcimFolderMouseArea = root->findChild<QQuickItem*>("dcimFolderMouseArea");
    QQuickItem* createProjectBackButton = root->findChild<QQuickItem*>("createProjectBackButton");

    if (sdCardMouseArea == nullptr || dcimFolderMouseArea == nullptr || createProjectBackButton == nullptr) {
        Dialogs::critical(
            "Error loading the project creator",
            "Could not find QML objects"
        );
        return;
    }

    connect(sdCardMouseArea, SIGNAL(clickDetected()), this, SLOT(sdCardClicked()));
    connect(dcimFolderMouseArea, SIGNAL(clickDetected()), this, SLOT(folderClicked()));
    connect(createProjectBackButton, SIGNAL(clicked()), this, SLOT(backButtonClicked()));
}

ProjectCreator::~ProjectCreator()
{
    delete ui;
}

void ProjectCreator::sdCardClicked()
{
    emit changePane(new ProjectCreatorSd(mainWindow));
}

void ProjectCreator::folderClicked()
{
    emit changePane(new ProjectCreatorFolder(mainWindow));
}

void ProjectCreator::backButtonClicked()
{
    emit changePane(new WelcomePane(mainWindow));
}
