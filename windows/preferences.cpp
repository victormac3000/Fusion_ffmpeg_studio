#include "preferences.h"
#include "ui_preferences.h"

Preferences::Preferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferences)
{
    ui->setupUi(this);

    setWindowTitle("Settings");

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
        appDataPathBrowseButton == nullptr)
    {
        qWarning() << "Could not find QML objects for the preferences areas";
        return;
    }

    QQuickItem *changeAppDataPathButton = preferencesGeneralArea->findChild<QQuickItem*>("appDataPathBrowseButton");

    if (changeAppDataPathButton == nullptr) {
        qWarning() << "Could not find QML objects from preferences general area";
        return;
    }

    preferencesGeneralArea->setProperty("appDataPath", Settings::getAppDataPath());
    preferencesGeneralArea->setProperty("defaultProjectName", "defaultProjectNameTest");

    connect(changeAppDataPathButton, SIGNAL(clicked()), this, SLOT(changeAppDataDir()));

    codecsComboBox = preferencesRenderingArea->findChild<QQuickItem*>("codecsComboBox");
    encodersComboBox = preferencesRenderingArea->findChild<QQuickItem*>("encodersComboBox");
    formatsComboBox = preferencesRenderingArea->findChild<QQuickItem*>("formatsComboBox");


    if (codecsComboBox == nullptr || encodersComboBox == nullptr ||
        formatsComboBox == nullptr)
    {
        qWarning() << "Could not find QML objects from preferences rendering area";
        return;
    }

    connect(codecsComboBox, SIGNAL(save(QString,QString)), this, SLOT(handleSave(QString,QString)));
    connect(codecsComboBox, SIGNAL(codecChanged()), this, SLOT(handleCodecChanged()));
    connect(encodersComboBox, SIGNAL(save(QString,QString)), this, SLOT(handleSave(QString,QString)));
    connect(formatsComboBox, SIGNAL(save(QString,QString)), this, SLOT(handleSave(QString,QString)));

    QStringList availableCodecs = Settings::getAvailableCodecs();
    QString defaultCodec = Settings::getDefaultCodec();

    QMetaObject::invokeMethod(
        codecsComboBox,
        "add",
        Q_ARG(QVariant, availableCodecs),
        Q_ARG(QVariant, defaultCodec)
    );

    handleSave(defaultCodec, "codec");
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

    QString copyAppDataErr = copyAppData(proposedAppDataDir);

    if (!copyAppDataErr.isEmpty()) {
        Dialogs::warning("Could not change the appData directory: " + copyAppDataErr,
                        "Failed to copy appdata contents to the new path: " + copyAppDataErr);
        return;
    }

    preferencesGeneralArea->setProperty("appDataPath", proposedAppDataDir);
    settings.setValue("appData", proposedAppDataDir);
}

void Preferences::handleSave(QString data, QString type)
{
    if (type == "codec") {
        Settings::setDefaultCodec(data);

        QMetaObject::invokeMethod(
            encodersComboBox,
            "add",
            Q_ARG(QVariant, QVariant::fromValue(Settings::getAvailableEncoders(Settings::getDefaultCodec()))),
            Q_ARG(QVariant, Settings::getDefaultEncoder())
        );

        QMetaObject::invokeMethod(
            formatsComboBox,
            "add",
            Q_ARG(QVariant, QVariant::fromValue(Settings::getAvailableFormats(Settings::getDefaultCodec()))),
            Q_ARG(QVariant, Settings::getDefaultFormat())
        );
    }
    if (type == "encoder") {
        Settings::setDefaultEncoder(data);
    }
    if (type == "format") {
        Settings::setDefaultFormat(data);
    }
}

void Preferences::handleCodecChanged()
{
    QStringList availableEncoders = Settings::getAvailableEncoders(Settings::getDefaultCodec());
    QString defaultEncoder = Settings::getDefaultEncoder();

    QMetaObject::invokeMethod(
        encodersComboBox,
        "add",
        Q_ARG(QVariant, availableEncoders),
        Q_ARG(QVariant, defaultEncoder)
    );

    QStringList availableFormats = Settings::getAvailableFormats(Settings::getDefaultCodec());
    QString defaultFormat = Settings::getDefaultFormat();

    QMetaObject::invokeMethod(
        formatsComboBox,
        "add",
        Q_ARG(QVariant, availableFormats),
        Q_ARG(QVariant, defaultFormat)
    );
}

QString Preferences::copyAppData(QString path)
{
    QString appDataPath = Settings::getAppDataPath();

    QDir dir(appDataPath);
    QDir newDir(path);

    if (!dir.exists() || !dir.isReadable()) {
        qCritical() << "Actual appdata does not exist";
        return "Critical error occurred";
    }

    if (!newDir.exists() || !QFileInfo(path).isWritable()) {
        qWarning() << "Could not move appdata to a non writable directory";
        return "You must select a writable directory";
    }

    if (!newDir.isEmpty()) {
        return "You must select an empty directory";
    }

    QFileInfoList dirFiles = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);

    for (const QFileInfo &dirFileInfo: dirFiles) {
        QString src = dirFileInfo.absoluteFilePath();
        QString dst = newDir.absolutePath() + "/" + dirFileInfo.fileName();
        if (QFile::exists(dst)) {
            qWarning() << "File exists in destination and shouldnt exist: "<< dst;
            return "Error copying the files. View the logs for more information";
        }
        if (!QFile(src).copy(dst)) {
            qWarning() << "Could not copy file "<< src << " to " << dst;
            return "Error copying the files. View the logs for more information";
        }
    }

    return "";
}
