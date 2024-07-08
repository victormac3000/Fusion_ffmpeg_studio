#include "about.h"
#include "QtQuick/qquickitem.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    if (ui->quickWidget == nullptr) {
        qWarning() << "About could not found quickWidget element";
        return;
    }

    QQuickItem* root = ui->quickWidget->rootObject();

    if (root == nullptr) {
        qWarning() << "About could not found quickWidget element root QML object";
        return;
    }

    QQuickItem* versionText = root->findChild<QQuickItem*>("versionText");
    QQuickItem* licenseTextEdit = root->findChild<QQuickItem*>("licenseTextEdit");

    if (versionText == nullptr || licenseTextEdit == nullptr) {
        qWarning() << "Could not find QML objects";
        return;
    }

    versionText->setProperty("text", QCoreApplication::applicationVersion());
    licenseTextEdit->setProperty("text", getMainLicenseText());
}

About::~About()
{
    delete ui;
}

QString About::getMainLicenseText()
{
    QFile licenseFile(":/Documents/Txt/LICENSE");
    QString licenseText = "Could not find license contents";

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
