#include "preferences.h"
#include "ui_preferences.h"

Preferences::Preferences(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Preferences)
{
    ui->setupUi(this);
    error = new ErrorHandler(this);
    connect(ui->change_working_dir, SIGNAL(clicked()), this, SLOT(changeWorkingDir()));
    connect(ui->change_rendered_dir, SIGNAL(clicked()), this, SLOT(changeRenderedDir()));
    if (settings.contains("workingDir") && !settings.value("workingDir").toString().isEmpty()) {
        ui->working_dir->setText(settings.value("workingDir").toString());
    }
    if (settings.contains("renderedDir") && !settings.value("renderedDir").toString().isEmpty()) {
        ui->rendered_dir->setText(settings.value("renderedDir").toString());
    }
}

Preferences::~Preferences()
{
    delete ui;
}

void Preferences::changeWorkingDir()
{
    QString proposedWd = QFileDialog::getExistingDirectory(
        this, tr("Select working directory"), "", QFileDialog::ShowDirsOnly
    );
    if (proposedWd.isEmpty()) return;

    if (!canWrite(proposedWd)) {
        error->warning("Cannot change the working directory:\n" + errorMsg);
        return;
    }

    ui->working_dir->setText(proposedWd);
    settings.setValue("workingDir", proposedWd);
}

void Preferences::changeRenderedDir()
{
    QString proposedRd = QFileDialog::getExistingDirectory(
        this, tr("Select rendered directory"), "", QFileDialog::ShowDirsOnly
    );
    if (proposedRd.isEmpty()) return;

    if (!canWrite(proposedRd)) {
        error->warning("Cannot change the rendered directory:\n" + errorMsg);
        return;
    }

    ui->rendered_dir->setText(proposedRd);
    settings.setValue("renderedDir", proposedRd);
}

bool Preferences::canWrite(QString path)
{
    QDir dir(path);

    if (!dir.exists()) {
        errorMsg = "The directory (" + path + ") does not exist on the system";
        return false;
    }

    QFile file(path + "testWrite");

    if (file.exists()) {
        if (!file.remove()) {
            errorMsg = "Cannot write data in the directory (" + path + "), cannot delete old test write file";
            return false;
        }
    }

    if (!file.open(QFile::ReadWrite | QFile::NewOnly)) {
        errorMsg = "Cannot write data in the directory (" + path + ")";
        return false;
    }

    return true;
}
