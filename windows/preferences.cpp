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
        appDataPathBrowseButton == nullptr)
    {
        qWarning() << "Could not find QML objects for the preferences areas";
        return;
    }

    QQuickItem *changeAppDataPathButton = preferencesGeneralArea->findChild<QQuickItem*>("appDataPathBrowseButton");
    QQuickItem *fontSizeScaleSlider = preferencesGeneralArea->findChild<QQuickItem*>("fontSizeScaleSlider");

    if (changeAppDataPathButton == nullptr || fontSizeScaleSlider == nullptr) {
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

    if (!copyAppData(proposedAppDataDir)) {
        Dialogs::warning("Cannot change the rendered directory, failed to copy appdata contents to the new path");
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
