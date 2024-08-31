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

    QQuickItem* frontSDRectangle = root->findChild<QQuickItem*>("frontSDRectangle");
    QQuickItem* frontSDComboBox = root->findChild<QQuickItem*>("frontSDComboBox");
    QQuickItem* backSDRectangle = root->findChild<QQuickItem*>("backSDRectangle");
    QQuickItem* backSDComboBox = root->findChild<QQuickItem*>("backSDComboBox");
    QQuickItem* createProjectSDBackButton = root->findChild<QQuickItem*>("createProjectSDBackButton");
    QQuickItem* createProjectSDButton = root->findChild<QQuickItem*>("createProjectSDButton");

    if (frontSDRectangle == nullptr || frontSDComboBox == nullptr ||
        backSDRectangle == nullptr || backSDComboBox == nullptr ||
        createProjectSDBackButton == nullptr || createProjectSDButton == nullptr) {
        qWarning() << "Could not find QML objects";
        return;
    }

    QStringList mountedVolumes = MyQSysInfo::mountedVolumes(false, true);

    if (mountedVolumes.length() > 0) {
        QMetaObject::invokeMethod(
            frontSDComboBox,
            "add",
            Q_ARG(QVariant, mountedVolumes),
            Q_ARG(QVariant, mountedVolumes.first())
        );
        QMetaObject::invokeMethod(
            backSDComboBox,
            "add",
            Q_ARG(QVariant, mountedVolumes),
            Q_ARG(QVariant, mountedVolumes.first())
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
