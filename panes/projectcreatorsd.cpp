#include "projectcreatorsd.h"
#include "ui_projectcreatorsd.h"
#include "windows/mainwindow.h"

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
    QQuickItem* createProjectSDButton = root->findChild<QQuickItem*>("createProjectSDButton");

    if (frontSDRectangle == nullptr || frontSDComboBox == nullptr ||
        backSDRectangle == nullptr || backSDComboBox == nullptr ||
        createProjectSDBackButton == nullptr || createProjectSDButton == nullptr) {
        qWarning() << "Could not find QML objects";
        return;
    }

    connect(frontSDComboBox, SIGNAL(addCompleted()), this, SLOT(checkFrontSelection()));
    connect(frontSDComboBox, SIGNAL(currentIndexChanged()), this, SLOT(checkFrontSelection()));
    connect(backSDComboBox, SIGNAL(addCompleted()), this, SLOT(checkBackSelection()));
    connect(backSDComboBox, SIGNAL(currentIndexChanged()), this, SLOT(checkBackSelection()));


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

void ProjectCreatorSd::checkFrontSelection()
{
    if (frontSDComboBox == nullptr || frontSDRectangle == nullptr) {
        Dialogs::warning("Error checking the volumes", "front SD combobox and/or rectangle are null");
        return;
    }

    QString selectedText = frontSDComboBox->property("currentText").toString();
    qDebug() << "FRONT: " + selectedText;
    for (const VolumeInfo &volume: mountedVolumes) {
        bool volumeMatches = selectedText == volume.label;
        bool hasBasicPaths = QDir(volume.mountPath + "/DCIM/100GFRNT").exists();
        if (volumeMatches && hasBasicPaths) {
            frontSDRectangle->setProperty("valid", true);
            return;
        }
    }
    frontSDRectangle->setProperty("valid", false);
}

void ProjectCreatorSd::checkBackSelection()
{
    if (backSDComboBox == nullptr || backSDRectangle == nullptr) {
        Dialogs::warning("Error checking the volumes", "back SD combobox and/or rectangle are null");
        return;
    }

    QString selectedText = backSDComboBox->property("currentText").toString();
    qDebug() << "BACK: " + selectedText;
    for (const VolumeInfo &volume: mountedVolumes) {
        bool volumeMatches = selectedText == volume.label;
        bool hasBasicPaths = QDir(volume.mountPath + "/DCIM/100GBACK").exists();
        if (volumeMatches && hasBasicPaths) {
            backSDRectangle->setProperty("valid", true);
            return;
        }
    }
    backSDRectangle->setProperty("valid", false);
}
