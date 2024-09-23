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
    QQuickItem* createProjectSDBackButton = root->findChild<QQuickItem*>("createProjectSDBackButton");
    createProjectSDButton = root->findChild<QQuickItem*>("createProjectSDButton");

    if (frontSDRectangle == nullptr || frontSDComboBox == nullptr ||
        backSDRectangle == nullptr || backSDComboBox == nullptr ||
        createProjectSDBackButton == nullptr || createProjectSDButton == nullptr) {
        qWarning() << "Could not find QML objects";
        return;
    }

    connect(frontSDComboBox, SIGNAL(addCompleted(QString)), this, SLOT(checkFrontSelection(QString)));
    connect(frontSDComboBox, SIGNAL(optionChanged(QString)), this, SLOT(checkFrontSelection(QString)));
    connect(backSDComboBox, SIGNAL(addCompleted(QString)), this, SLOT(checkBackSelection(QString)));
    connect(backSDComboBox, SIGNAL(optionChanged(QString)), this, SLOT(checkBackSelection(QString)));
    connect(createProjectSDBackButton, SIGNAL(clicked()), this, SLOT(backButtonClicked()));
    connect(createProjectSDButton, SIGNAL(clicked()), this, SLOT(createButtonClicked()));

    mountedVolumes = MyQSysInfo::mountedVolumes();
    QStringList volumeLabels;

    for (const VolumeInfo &volume: mountedVolumes) {
        if (volume.isExternal) {
            volumeLabels.append(volume.label);
        }
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
            validateSelections();
            return;
        }
    }

    backSDRectangle->setProperty("valid", false);
    validateSelections();
}

void ProjectCreatorSd::createButtonClicked()
{
    LoadingInfo loadingInfo;
    loadingInfo.type = CREATE_PROJECT_SD;


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

    bool frontValid = frontSDRectangle->property("valid").toBool();
    bool backValid = backSDRectangle->property("valid").toBool();
    bool valid = frontValid && backValid;

    createProjectSDButton->setProperty("enabled", valid);
}
