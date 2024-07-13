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

    preferencesGeneralArea = root->findChild<QQuickItem*>("preferencesGeneralArea");
    preferencesRenderingArea = root->findChild<QQuickItem*>("preferencesRenderingArea");
    QQuickItem* appDataPathBrowseButton = root->findChild<QQuickItem*>("appDataPathBrowseButton");


    if (preferencesGeneralArea == nullptr || preferencesRenderingArea == nullptr ||
        appDataPathBrowseButton == nullptr) {
        qWarning() << "Could not find QML objects";
        return;
    }

    connect(preferencesRenderingArea, SIGNAL(requestEncoders(QString)), this, SLOT(handleRequestEncoders(QString)));
    connect(preferencesRenderingArea, SIGNAL(requestVideoFormats(QString)), this, SLOT(handleRequestVideoFormats(QString)));
    connect(preferencesRenderingArea, SIGNAL(save(QString,QString)), this, SLOT(handleSave(QString,QString)));
    connect(appDataPathBrowseButton, SIGNAL(clicked()), this, SLOT(changeAppDataDir()));

    // TODO manage all the settings

    preferencesGeneralArea->setProperty("appDataPath", "appDataTest");
    preferencesGeneralArea->setProperty("defaultProjectName", "defaultProjectNameTest");


    QMetaObject::invokeMethod(
        preferencesRenderingArea,
        "addCodecs",
        Q_ARG(QVariant, QVariant::fromValue(Settings::getAvailableCodecs())),
        Q_ARG(QVariant, Settings::getDefaultCodec())
    );
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

void Preferences::handleRequestEncoders(QString codec)
{
    QMetaObject::invokeMethod(
        preferencesRenderingArea,
        "addEncoders",
        Q_ARG(QVariant, QVariant::fromValue(Settings::getAvailableEncoders(codec))),
        Q_ARG(QVariant, Settings::getDefaultEncoder())
    );
}

void Preferences::handleRequestVideoFormats(QString codec)
{
    Settings::resetDefaultFormat();
    QMetaObject::invokeMethod(
        preferencesRenderingArea,
        "addFormats",
        Q_ARG(QVariant, QVariant::fromValue(Settings::getAvailableFormats(codec))),
        Q_ARG(QVariant, Settings::getDefaultFormat())
    );
}

void Preferences::handleSave(QString data, QString type)
{
    if (type == "codec") {
        Settings::setDefaultCodec(data);
    }
    if (type == "encoder") {
        Settings::setDefaultEncoder(data);
    }
    if (type == "format") {
        Settings::setDefaultFormat(data);
    }
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
