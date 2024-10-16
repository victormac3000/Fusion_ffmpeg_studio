#include "ui_projectcreatorsd.h"
#include "panes/projectcreatorsd.h"
#include "utils/myqsysinfo.h"
#include "panes/projectcreator.h"
#include "panes/loadingpane.h"
#include "windows/mainwindow.h"
#include "utils/dialogs.h"
#include "models/loading.h"

ProjectCreatorSd::ProjectCreatorSd(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProjectCreatorSd)
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

    frontSDRectangle = root->findChild<QQuickItem*>("frontSDRectangle");
    frontSDComboBox = root->findChild<QQuickItem*>("frontSDComboBox");
    backSDRectangle = root->findChild<QQuickItem*>("backSDRectangle");
    backSDComboBox = root->findChild<QQuickItem*>("backSDComboBox");
    projectNameField = root->findChild<QQuickItem*>("projectNameField");
    projectPathField = root->findChild<QQuickItem*>("projectPathField");
    projectPathButton = root->findChild<QQuickItem*>("projectPathButton");

    createProjectSDBackButton = root->findChild<QQuickItem*>("createProjectSDBackButton");
    createProjectSDButton = root->findChild<QQuickItem*>("createProjectSDButton");

    if (frontSDRectangle == nullptr || frontSDComboBox == nullptr ||
        backSDRectangle == nullptr || backSDComboBox == nullptr ||
        createProjectSDBackButton == nullptr || createProjectSDButton == nullptr ||
        projectNameField == nullptr || projectPathField == nullptr || projectPathButton == nullptr) {
        qWarning() << "Could not find QML objects";
        return;
    }

    connect(frontSDComboBox, SIGNAL(addCompleted(QString)), this, SLOT(checkFrontSelection(QString)));
    connect(frontSDComboBox, SIGNAL(optionChanged(QString)), this, SLOT(checkFrontSelection(QString)));
    connect(backSDComboBox, SIGNAL(addCompleted(QString)), this, SLOT(checkBackSelection(QString)));
    connect(backSDComboBox, SIGNAL(optionChanged(QString)), this, SLOT(checkBackSelection(QString)));
    connect(createProjectSDBackButton, SIGNAL(clicked()), this, SLOT(backButtonClicked()));
    connect(createProjectSDButton, SIGNAL(clicked()), this, SLOT(createButtonClicked()));
    connect(projectPathButton, SIGNAL(clicked()), this, SLOT(browseProjectLocationClicked()));
    connect(projectNameField, SIGNAL(textChanged()), this, SLOT(projectNameChanged()));

    generateDefaultProject();

    mountedVolumes = MyQSysInfo::mountedVolumes();
    QStringList volumeLabels;

    for (const VolumeInfo &volume: mountedVolumes) {
        #ifdef QT_DEBUG
        volumeLabels.append(volume.label);
        #else
        if (volume.isExternal) {
            volumeLabels.append(volume.label);
        }
        #endif
    }

    if (volumeLabels.length() > 0) {
        QMetaObject::invokeMethod(
            frontSDComboBox,
            "add",
            Q_ARG(QVariant, volumeLabels),
            Q_ARG(QVariant, volumeLabels.first())
        );
        QMetaObject::invokeMethod(
            backSDComboBox,
            "add",
            Q_ARG(QVariant, volumeLabels),
            Q_ARG(QVariant, volumeLabels.first())
        );
    }

    initOk = true;
}

ProjectCreatorSd::~ProjectCreatorSd()
{
    delete ui;
}

bool ProjectCreatorSd::getInit()
{
    return initOk;
}

void ProjectCreatorSd::backButtonClicked()
{
    emit changePane(new ProjectCreator(mainWindow));
}

void ProjectCreatorSd::checkFrontSelection(QString selectedText)
{
    if (frontSDComboBox == nullptr || frontSDRectangle == nullptr) {
        Dialogs::warning("Error checking the volumes", "front SD combobox and/or rectangle are null");
        return;
    }

    for (const VolumeInfo &volume: mountedVolumes) {
        bool volumeMatches = selectedText == volume.label;
        bool hasBasicPaths = QDir(volume.mountPath + "/DCIM/100GFRNT").exists();
        if (volumeMatches && hasBasicPaths) {
            frontSDRectangle->setProperty("valid", true);
            frontVolume = volume;
            validateSelections();
            return;
        }
    }

    frontSDRectangle->setProperty("valid", false);
    validateSelections();
}

void ProjectCreatorSd::checkBackSelection(QString selectedText)
{
    if (backSDComboBox == nullptr || backSDRectangle == nullptr) {
        Dialogs::warning("Error checking the volumes", "back SD combobox and/or rectangle are null");
        return;
    }

    for (const VolumeInfo &volume: mountedVolumes) {
        bool volumeMatches = selectedText == volume.label;
        bool hasBasicPaths = QDir(volume.mountPath + "/DCIM/100GBACK").exists();
        if (volumeMatches && hasBasicPaths) {
            backSDRectangle->setProperty("valid", true);
            backVolume = volume;
            validateSelections();
            return;
        }
    }

    backSDRectangle->setProperty("valid", false);
    validateSelections();
}

void ProjectCreatorSd::createButtonClicked()
{
    QString projectName = projectNameField->property("text").toString();
    bool frontPath = frontSDRectangle->property("valid").toBool();
    bool backPath = backSDRectangle->property("valid").toBool();

    if (projectName.isEmpty()) {
        Dialogs::warning("The project name cannot be empty");
        return;
    }

    if (QDir(rootProjectFolder).exists(projectName)) {
        Dialogs::warning("The project name already exists");
        return;
    }

    if (!frontPath || frontVolume.mountPath.isEmpty()) {
        Dialogs::warning("You must select a valid front sd");
        return;
    }

    if (!backPath || backVolume.mountPath.isEmpty()) {
        Dialogs::warning("You must select a valid back sd");
        return;
    }

    LoadingInfo loadingInfo;
    loadingInfo.type = CREATE_PROJECT_SD;
    loadingInfo.rootProjectPath = rootProjectFolder;
    loadingInfo.projectName = projectName;
    loadingInfo.projectPath = rootProjectFolder + "/" + projectName;
    loadingInfo.dcimFrontPath = frontVolume.mountPath + "/DCIM/100GFRNT";
    loadingInfo.dcimBackPath = backVolume.mountPath + "/DCIM/100GBACK";
    loadingInfo.copyDCIM = true;

    LoadingPane* loader = new LoadingPane(mainWindow, loadingInfo);
    if (loader->getInit()) {
        emit changePane(loader);
    } else {
        Dialogs::warning("Could not load the next menu");
    }
}

void ProjectCreatorSd::validateSelections()
{
    if (createProjectSDButton == nullptr) {
        Dialogs::warning("Error checking the volumes", "create project button is nullptr");
        return;
    }

    if (frontSDRectangle == nullptr) {
        Dialogs::warning("Error checking the volumes", "frontSDRectangle is nullptr");
        return;
    }

    if (backSDRectangle == nullptr) {
        Dialogs::warning("Error checking the volumes", "backSDRectangle is nullptr");
        return;
    }

    bool frontValid = frontSDRectangle->property("valid").toBool();
    bool backValid = backSDRectangle->property("valid").toBool();
    bool valid = frontValid && backValid;

    createProjectSDButton->setProperty("enabled", valid);
}

void ProjectCreatorSd::projectNameChanged()
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

void ProjectCreatorSd::browseProjectLocationClicked()
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

void ProjectCreatorSd::generateDefaultProject()
{
    generateDefaultProjectPath();

    QString defaultProjectName = generateDefaultProjectName();

    projectNameField->setProperty("text", defaultProjectName);
    projectPathField->setProperty("text", rootProjectFolder + "/" + defaultProjectName);
}

void ProjectCreatorSd::generateDefaultProjectPath()
{
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QFileInfo documentsPathInfo(documentsPath);

    if (!documentsPathInfo.isWritable()) {
        qWarning() << "documents path is not writable";
        return;
    }

    rootProjectFolder = documentsPath;
}

QString ProjectCreatorSd::generateDefaultProjectName()
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
