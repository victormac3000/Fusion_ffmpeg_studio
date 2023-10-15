#include "projectcreator.h"
#include "ui_projectcreator.h"
#include "windows/mainwindow.h"

ProjectCreator::ProjectCreator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectCreator)
{
    ui->setupUi(this);
    ui->project_location->setText(settings.value("defaultProjectPath").toString());
    this->mainWindowWidget = parent;

    // Connect buttons
    connect(ui->project_name, SIGNAL(textChanged(QString)), this, SLOT(projectNameChanged(QString)));
    connect(ui->browse_dcim_location, SIGNAL(clicked()), this, SLOT(browseDCIMFolderClicked()));
    connect(ui->browse_project_location, SIGNAL(clicked()), this, SLOT(browseProjectLocationClicked()));
    connect(ui->cancel_button, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));
    connect(ui->create_button, SIGNAL(clicked()), this, SLOT(createButtonClicked()));
}

ProjectCreator::~ProjectCreator()
{
    delete ui;
}

void ProjectCreator::projectNameChanged(QString newName)
{
    ui->project_location->setText(settings.value("defaultProjectPath").toString() + "/" + newName);
}

void ProjectCreator::browseDCIMFolderClicked()
{
    QString proposedDCIMFolder = QFileDialog::getExistingDirectory(
        this, tr("Select the DCIM directory"), "/Users/victor/Documents/NoTM/2023_02_11_Nieve/Test/DCIM", QFileDialog::ShowDirsOnly
    );
    ui->dcim_location->setText(proposedDCIMFolder);
}

void ProjectCreator::browseProjectLocationClicked()
{
    QString proposedProjectFolder = QFileDialog::getExistingDirectory(
        this, tr("Select the project directory"), "/Users/victor/Documents/NoTM/2023_02_11_Nieve/Test/Project", QFileDialog::ShowDirsOnly
    );
    ui->project_location->setText(proposedProjectFolder + "/" + ui->project_name->text());
}

void ProjectCreator::cancelButtonClicked()
{
    emit changePane(new WelcomePane((MainWindow*) parent()));
}

void ProjectCreator::createButtonClicked()
{
    QString projectName = ui->project_name->text();
    QString DCIMFolder = ui->dcim_location->text();
    QString projectFolder = ui->project_location->text();

    // Check that the strings are not empty

    if (projectName.isEmpty()) {
        Dialogs::warning("The project name cannot be empty");
        return;
    }

    if (DCIMFolder.isEmpty()) {
        Dialogs::warning("The project DCIM folder cannot be empty");
        return;
    }

    if (projectFolder.isEmpty()) {
        Dialogs::warning("The project folder cannot be empty");
        return;
    }

    LoadingPane *loader = new LoadingPane(mainWindowWidget, DCIMFolder, projectName, projectFolder);
    emit changePane(loader);
}
