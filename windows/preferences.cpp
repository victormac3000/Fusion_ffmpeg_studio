#include "preferences.h"
#include "ui_preferences.h"

Preferences::Preferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferences)
{
    ui->setupUi(this);
    connect(ui->change_appdata, SIGNAL(clicked()), this, SLOT(changeAppDataDir()));
    ui->appdata->setText(settings.value("appData").toString());
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

    ui->appdata->setText(proposedAppDataDir);
    settings.setValue("appData", proposedAppDataDir);
}

bool Preferences::copyAppData(QString path)
{
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
