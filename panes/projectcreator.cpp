#include "projectcreator.h"
#include "ui_projectcreator.h"
#include "windows/mainwindow.h"
#include "panes/welcomepane.h"
#include "panes/projectcreatorfolder.h"
#include "panes/projectcreatorsd.h"
#include "utils/dialogs.h"

ProjectCreator::ProjectCreator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectCreator)
{
    ui->setupUi(this);

    this->mainWindow = (MainWindow*) parent;

    if (mainWindow == nullptr) {
        qWarning() << "MainWindow pointer is null";
        return;
    }

    mainWindow->setTitle("Create project");

    QQuickItem* root = ui->quickWidget->rootObject();

    if (root == nullptr) {
        qWarning() << "Could not found quickWidget element root QML object.";
        return;
    }

    QQuickItem* sdCardMouseArea = root->findChild<QQuickItem*>("sdCardMouseArea");
    QQuickItem* dcimFolderMouseArea = root->findChild<QQuickItem*>("dcimFolderMouseArea");
    QQuickItem* createProjectBackButton = root->findChild<QQuickItem*>("createProjectBackButton");

    if (sdCardMouseArea == nullptr || dcimFolderMouseArea == nullptr || createProjectBackButton == nullptr) {
        qWarning() << "Could not find QML objects";
        return;
    }

    connect(sdCardMouseArea, SIGNAL(clickDetected()), this, SLOT(sdCardClicked()));
    connect(dcimFolderMouseArea, SIGNAL(clickDetected()), this, SLOT(folderClicked()));
    connect(createProjectBackButton, SIGNAL(clicked()), this, SLOT(backButtonClicked()));

    initOk = true;
}

ProjectCreator::~ProjectCreator()
{
    delete ui;
}

bool ProjectCreator::getInit()
{
    return initOk;
}

void ProjectCreator::sdCardClicked()
{
    ProjectCreatorSd* projectCreatorSd = new ProjectCreatorSd(mainWindow);
    if (projectCreatorSd->getInit()) {
        emit changePane(projectCreatorSd);
    } else {
        Dialogs::warning("Could not load menu");
    }
}

void ProjectCreator::folderClicked()
{
    ProjectCreatorFolder* projectCreatorFolder = new ProjectCreatorFolder(mainWindow);
    if (projectCreatorFolder->getInit()) {
        emit changePane(projectCreatorFolder);
    } else {
        Dialogs::warning("Could not load menu");
    }
}

void ProjectCreator::backButtonClicked()
{
    emit changePane(new WelcomePane(mainWindow));
}
