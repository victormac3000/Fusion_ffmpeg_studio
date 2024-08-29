#include "about.h"
#include "QtQuick/qquickitem.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    QQuickItem* root = ui->quickWidget->rootObject();

    if (root == nullptr) {
        qWarning() << "About could not found quickWidget element root QML object";
        return;
    }

    QQuickItem* versionText = root->findChild<QQuickItem*>("versionText");
    QQuickItem* licenseTextArea = root->findChild<QQuickItem*>("licenseTextArea");
    QQuickItem* librariesInfoGrid = root->findChild<QQuickItem*>("librariesInfoGrid");
    QQuickItem* buildInfoGrid = root->findChild<QQuickItem*>("buildInfoGrid");


    if (versionText == nullptr || licenseTextArea == nullptr || librariesInfoGrid == nullptr ||
        buildInfoGrid == nullptr) {
        qWarning() << "Could not find QML objects";
        return;
    }

    QString buildDatetime = QStringLiteral(__DATE__) + " " + QStringLiteral(__TIME__);
    QString compilationType = "Release";
    #ifdef QT_DEBUG
    compilationType = "Debug";
    #endif

    // TODO obtain ffmpeg version

    versionText->setProperty("text", QCoreApplication::applicationVersion());
    licenseTextArea->setProperty("areaText", getLicenseText());
    librariesInfoGrid->setProperty("qtVersion", QT_VERSION_STR);
    librariesInfoGrid->setProperty("ffmpegVersion", "N/A");
    buildInfoGrid->setProperty("compilationDate", buildDatetime);
    buildInfoGrid->setProperty("compilationType", compilationType);
    buildInfoGrid->setProperty("compilationOSName", QSysInfo::productType());
    buildInfoGrid->setProperty("compilationOSVersion", QSysInfo::productVersion());

    init = true;
}

About::~About()
{
    delete ui;
}

bool About::getInit()
{
    return init;
}

QString About::getLicenseText(int license)
{
    QString licenseText = "Could not find license contents";
    QString licensePath = "";

    switch (license) {
        case LICENSE_MAIN:
            licensePath = ":/Documents/Txt/LICENSE";
            break;
        case LICENSE_LIBRARIES:
            licensePath = ":/Documents/Txt/LICENSE_LIBS";
            break;
        default:
            return licenseText;
            break;
    }

    QFile licenseFile(licensePath);

    if (!licenseFile.open(QFile::ReadOnly)) {
        qWarning() << "Could not open main license file";
        return licenseText;
    }

    if (licenseFile.size() < 1) {
        qWarning() << "The license file is empty";
        return licenseText;
    }

    licenseText = licenseFile.readAll();
    licenseFile.close();

    return licenseText;
}
