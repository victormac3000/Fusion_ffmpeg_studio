#include "preferences.h"
#include "ui_preferences.h"

Preferences::Preferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferences)
{
    ui->setupUi(this);

    if (ui->quickWidget == nullptr) {
        qWarning() << "Settings could not found quickWidget element";
        return;
    }

    QQuickItem* root = ui->quickWidget->rootObject();

    if (root == nullptr) {
        qWarning() << "Settings could not found quickWidget element root QML object";
        return;
    }

    QQuickItem* preferencesGeneralArea = root->findChild<QQuickItem*>("preferencesGeneralArea");
    QQuickItem* preferencesRenderingArea = root->findChild<QQuickItem*>("preferencesRenderingArea");
    QQuickItem* appDataPathBrowseButton = root->findChild<QQuickItem*>("appDataPathBrowseButton");


    if (preferencesGeneralArea == nullptr || preferencesRenderingArea == nullptr ||
        appDataPathBrowseButton == nullptr) {
        qWarning() << "Could not find QML objects";
        return;
    }

    // TODO manage all the settings

    preferencesGeneralArea->setProperty("appDataPath", "appDataTest");
    preferencesGeneralArea->setProperty("defaultProjectName", "defaultProjectNameTest");

    QStringList encoders {"A", "N", "X"};
    QStringList formats {"S", "D", "R"};

    QMetaObject::invokeMethod(preferencesRenderingArea, "addEncoders", Q_ARG(QVariant, QVariant::fromValue(encoders)));
    QMetaObject::invokeMethod(preferencesRenderingArea, "addFormats", Q_ARG(QVariant, QVariant::fromValue(formats)));


    connect(appDataPathBrowseButton, SIGNAL(clicked()), this, SLOT(changeAppDataDir()));
}

Preferences::~Preferences()
{
    delete ui;
}

void Preferences::changeAppDataDir()
{
    QString proposedAppDataDir = QFileDialog::getExistingDirectory(
        this, tr("Select new appdata directory"), "", QFileDialog::ShowDirsOnly
    );
    if (proposedAppDataDir.isEmpty()) return;

    if (!copyAppData(proposedAppDataDir)) {
        Dialogs::warning("Cannot change the rendered directory, failed to copy appdata contents to the new path");
        return;
    }

    //ui->appdata->setText(proposedAppDataDir);
    settings.setValue("appData", proposedAppDataDir);
}

bool Preferences::copyAppData(QString path)
{
    return false;
    // TODO fix
    QDir dir(settings.value("appData").toString());
    QDir newDir(path);

    QFileInfoList dirFiles = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);

    for (const QFileInfo &dirFileInfo: dirFiles) {
        QString src = dirFileInfo.absoluteFilePath();
        QString dst = newDir.absolutePath() + "/" + dirFileInfo.fileName();
        if (QFile::exists(dst) && !QFile::remove(dst)) {
            qWarning() << "Could not delete existing file in destination: "<< dst;
            return false;
        }
        if (!QFile(src).copy(dst)) {
            qWarning() << "Could not copy file "<< src << " to " << dst;
            return false;
        }
    }

    settings.setValue("appData", path);
    return true;
}
