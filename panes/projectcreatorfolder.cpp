#include "projectcreatorfolder.h"
#include "ui_projectcreatorfolder.h"
#include "windows/mainwindow.h"
#include "utils/dialogs.h"
#include "panes/projectcreator.h"
#include "panes/loadingpane.h"
#include "models/loading.h"

ProjectCreatorFolder::ProjectCreatorFolder(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProjectCreatorFolder)
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
        qWarning() << "Could not found quickWidget element root QML object";
        return;
    }

    projectNameField = root->findChild<QQuickItem*>("projectNameField");
    projectDCIMField = root->findChild<QQuickItem*>("projectDCIMField");
    projectPathField = root->findChild<QQuickItem*>("projectPathField");

    QQuickItem* projectPathButton = root->findChild<QQuickItem*>("projectPathButton");
    QQuickItem* projectDCIMButton = root->findChild<QQuickItem*>("projectDCIMButton");

    linkDCIMCheckbox = root->findChild<QQuickItem*>("linkDCIMCheckbox");
    copyDCIMCheckbox = root->findChild<QQuickItem*>("copyDCIMCheckbox");

    backButton = root->findChild<QQuickItem*>("createProjectFolderBackButton");
    createProjectButton = root->findChild<QQuickItem*>("createProjectFolderButton");

    if (projectNameField == nullptr || projectDCIMField == nullptr || projectPathField == nullptr ||
        projectPathButton == nullptr || projectDCIMButton == nullptr ||
        linkDCIMCheckbox == nullptr || copyDCIMCheckbox == nullptr ||
        backButton == nullptr || createProjectButton == nullptr) {
        qWarning() << "Could not find QML objects";
        return;
    }

    generateDefaultProject();

    connect(projectNameField, SIGNAL(textChanged()), this, SLOT(projectNameChanged()));

    connect(projectPathButton, SIGNAL(clicked()), this, SLOT(browseProjectLocationClicked()));
    connect(projectDCIMButton, SIGNAL(clicked()), this, SLOT(browseDCIMFolderClicked()));

    connect(backButton, SIGNAL(clicked()), this, SLOT(backButtonClicked()));
    connect(createProjectButton, SIGNAL(clicked()), this, SLOT(createButtonClicked()));

    initOk = true;
}

ProjectCreatorFolder::~ProjectCreatorFolder()
{
    delete ui;
}

void ProjectCreatorFolder::projectNameChanged()
{
    QString projectName = projectNameField->property("text").toString();

    if (rootProjectFolder.isEmpty()) {
        qWarning() << "root project folder is empty";
        return;
    }

    if (projectName.isEmpty()) {
        projectName = generateDefaultProjectName();
    }

    if (QDir(rootProjectFolder).exists(projectName)) {
        projectNameField->setProperty("color", "red");
        return;
    }

    if (projectName.length() > 50) {
        projectNameField->setProperty("color", "red");
        return;
    }

    projectNameField->setProperty("color", "#434A54");

    projectPathField->setProperty("text", rootProjectFolder + "/" + projectName);
}

void ProjectCreatorFolder::browseProjectLocationClicked()
{
    QString proposedProjectFolder = QFileDialog::getExistingDirectory(
        this, tr("Select the project directory"), "/Users/victor/Documents", QFileDialog::ShowDirsOnly
    );

    if (proposedProjectFolder.isEmpty()) return;

    QFileInfo dirInfo(proposedProjectFolder);
    if (!dirInfo.isWritable()) {
        Dialogs::warning("You must select a writable directory");
        return;
    }

    rootProjectFolder = proposedProjectFolder;

    projectPathField->setProperty("text", rootProjectFolder + "/" + projectNameField->property("text").toString());
}

void ProjectCreatorFolder::browseDCIMFolderClicked()
{
    QString proposedDCIMFolder = QFileDialog::getExistingDirectory(
        this, tr("Select the DCIM directory"), "/Volumes/MAQUINAS/Archivos/Test Fusion/DCIM", QFileDialog::ShowDirsOnly
    );

    if (proposedDCIMFolder.isEmpty()) return;

    if (!QFileInfo(proposedDCIMFolder).isReadable()) {
        Dialogs::warning("You must select a readable directory");
    }

    projectDCIMField->setProperty("text", proposedDCIMFolder);
}

void ProjectCreatorFolder::backButtonClicked()
{
    emit changePane(new ProjectCreator(mainWindow));
}

void ProjectCreatorFolder::createButtonClicked()
{
    QString projectName = projectNameField->property("text").toString();
    QString projectFolder = projectPathField->property("text").toString();
    QString dcimFolder = projectDCIMField->property("text").toString();
    bool indexDCIM = linkDCIMCheckbox->property("checked").toBool();
    bool copyDCIM = copyDCIMCheckbox->property("checked").toBool();

    if (projectName.isEmpty()) {
        Dialogs::warning("The project name cannot be empty");
        return;
    }

    if (QDir(rootProjectFolder).exists(projectName)) {
        Dialogs::warning("The project name already exists");
        return;
    }

    if (dcimFolder.isEmpty()) {
        Dialogs::warning("The project DCIM folder cannot be empty");
        return;
    }

    if (projectFolder.isEmpty()) {
        Dialogs::warning("The project folder cannot be empty");
        return;
    }

    if (!(indexDCIM ^ copyDCIM)) {
        Dialogs::warning("You must select to index or copy the DCIM folder");
        return;
    }

    backButton->setProperty("enabled", false);
    createProjectButton->setProperty("enabled", false);


    LoadingInfo loadingInfo;
    loadingInfo.type = CREATE_PROJECT_FOLDER;
    loadingInfo.rootProjectPath = rootProjectFolder;
    loadingInfo.projectName = projectName;
    loadingInfo.projectPath = rootProjectFolder + "/" + projectName;
    loadingInfo.dcimPath = dcimFolder;
    loadingInfo.copyDCIM = copyDCIM;

    LoadingPane* loader = new LoadingPane(mainWindow, loadingInfo);
    if (loader->getInit()) {
        emit changePane(loader);
    } else {
        Dialogs::warning("Could not load the next menu");
    }
}

bool ProjectCreatorFolder::getInit()
{
    return initOk;
}

void ProjectCreatorFolder::generateDefaultProject()
{
    generateDefaultProjectPath();

    QString defaultProjectName = generateDefaultProjectName();

    projectNameField->setProperty("text", defaultProjectName);
    projectPathField->setProperty("text", rootProjectFolder + "/" + defaultProjectName);
}

void ProjectCreatorFolder::generateDefaultProjectPath()
{
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QFileInfo documentsPathInfo(documentsPath);

    if (!documentsPathInfo.isWritable()) {
        qWarning() << "documents path is not writable";
        return;
    }

    rootProjectFolder = documentsPath;
}

QString ProjectCreatorFolder::generateDefaultProjectName()
{
    QString projectName;

    for (int i=0; i<DEFAULT_PROJECT_GENERATOR_MAX_ITERATIONS; i++) {
        QString proposedProjectName = "Untitled";
        if (i > 0) proposedProjectName += " " + QString::number(i);
        if (!QDir(rootProjectFolder).exists(proposedProjectName)) {
            projectName = proposedProjectName;
            break;
        }
    }

    if (projectName.isEmpty()) {
        qWarning() << "Could not find a default name for the project. Max iterations is "
                   << DEFAULT_PROJECT_GENERATOR_MAX_ITERATIONS;
        return "";
    }

    return projectName;
}
