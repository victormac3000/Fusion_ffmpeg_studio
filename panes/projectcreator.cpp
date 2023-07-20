#include "projectcreator.h"
#include "ui_projectcreator.h"
#include "windows/mainwindow.h"

ProjectCreator::ProjectCreator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectCreator)
{
    ui->setupUi(this);

    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString newDocumentsPath;

    for (int i=1; i<=1000; i++) {
        newDocumentsPath = documentsPath + "/ffs_project_" + QString::number(i);
        if (!QDir().exists(newDocumentsPath)) {
            settings.setValue("defaultProjectPath", newDocumentsPath);
            break;
        }
    }

    ui->project_location->setText(settings.value("defaultProjectPath").toString());

    // Connect buttons
    connect(ui->browse_dcim_location, SIGNAL(clicked()), this, SLOT(browseDCIMFolderClicked()));
    connect(ui->browse_project_location, SIGNAL(clicked()), this, SLOT(browseProjectLocationClicked()));
    connect(ui->cancel_button, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));
    connect(ui->create_button, SIGNAL(clicked()), this, SLOT(createButtonClicked()));
}

ProjectCreator::~ProjectCreator()
{
    delete ui;
}

void ProjectCreator::browseDCIMFolderClicked()
{
    QString proposedDCIMFolder = QFileDialog::getExistingDirectory(
        this, tr("Select the DCIM directory"), "/Users/victor/Documents/NoTM/2023_02_11_Nieve/Test/DCIM", QFileDialog::ShowDirsOnly
    );

    if (proposedDCIMFolder.isEmpty()) return;
    if (!QFileInfo(proposedDCIMFolder).isReadable()) {
        Dialogs::warning("The DCIM folder needs to be readable");
        qWarning() << "DCIM folder invalid, not readable" << proposedDCIMFolder;
        return;
    }

    ui->dcim_location->setText(proposedDCIMFolder);
}

void ProjectCreator::browseProjectLocationClicked()
{
    QString proposedProjectFolder = QFileDialog::getExistingDirectory(
        this, tr("Select the project directory"), "/Users/victor/Documents/NoTM/2023_02_11_Nieve/Test/Project", QFileDialog::ShowDirsOnly
    );

    if (proposedProjectFolder.isEmpty()) return;
    if (!QFileInfo(proposedProjectFolder).isWritable()) {
        Dialogs::warning("The project folder needs to be writable");
        qWarning() << "Project folder invalid, not writable" << proposedProjectFolder;
        return;
    }

    ui->project_location->setText(proposedProjectFolder);
}

void ProjectCreator::cancelButtonClicked()
{
    emit changePane(new WelcomePane);
}

void ProjectCreator::createButtonClicked()
{
    QString proposedDCIMFolder = ui->dcim_location->text();
    QString proposedProjectFolder = ui->project_location->text();

    if (proposedDCIMFolder.isEmpty()) {
        Dialogs::warning("The DCIM folder cannot be empty");
        qWarning() << "DCIM folder invalid, empty" << proposedDCIMFolder;
        return;
    }

    if (!QFileInfo(proposedDCIMFolder).isReadable()) {
        Dialogs::warning("The DCIM folder could not be read");
        qWarning() << "DCIM folder invalid, not readable" << proposedDCIMFolder;
        return;
    }

    if (proposedProjectFolder.isEmpty()) {
        Dialogs::warning("The project folder cannot be empty");
        qWarning() << "Project folder invalid, empty" << proposedProjectFolder;
        return;
    }

    if (!QFile::exists(proposedProjectFolder) && !QDir().mkdir(proposedProjectFolder)) {
        Dialogs::warning("The project folder could not be created");
        qWarning() << "Project folder invalid, not exists and mkdir failed" << proposedProjectFolder;
        return;
    }

    QDir projectFolder(proposedProjectFolder);

    if (!QFileInfo(proposedProjectFolder).isWritable()) {
        Dialogs::warning("The project folder must be writable");
        qWarning() << "Project folder invalid, not writable" << proposedProjectFolder;
        return;
    }

    if ((!projectFolder.exists("DFSegments") && !QDir(proposedProjectFolder).mkdir("DFSegments")) ||
        (!projectFolder.exists("DFVideos") && !QDir(proposedProjectFolder).mkdir("DFVideos")) ||
        (!projectFolder.exists("DFLowSegments") && !QDir(proposedProjectFolder).mkdir("DFLowSegments")) ||
        (!projectFolder.exists("DFLowVideos") && !QDir(proposedProjectFolder).mkdir("DFLowVideos")) ||
        (!projectFolder.exists("EVideos") && !QDir(proposedProjectFolder).mkdir("EVideos")) ||
        (!projectFolder.exists("ELowVideos") && !QDir(proposedProjectFolder).mkdir("ELowVideos"))) {
        Dialogs::warning("Could not create the project structure");
        qWarning() << "Project folder invalid, could not create project folders" << proposedProjectFolder;
        return;
    }

    QFile projectFile(proposedProjectFolder + "/project.ffs");

    if (!projectFile.open(QFile::ReadWrite)) {
        Dialogs::warning("Could not create the project file");
        qWarning() << "Project folder invalid, open project file failed" << proposedProjectFolder;
        return;
    }

    QJsonDocument doc;
    QJsonObject mainObj;

    QJsonObject info;

    info.insert("version", QJsonValue::fromVariant(QCoreApplication::applicationVersion()));
    info.insert("dcim", QJsonValue::fromVariant(proposedDCIMFolder));

    mainObj.insert("info", info);
    mainObj.insert("videos", QJsonArray());

    doc.setObject(mainObj);

    int written = projectFile.write(doc.toJson(QJsonDocument::Indented));
    if (written < 0) {
        Dialogs::warning("Could not create the project file");
        qWarning() << "Could not write project file, bytes written" << written << "into file" << projectFile.fileName();
        return;
    }

    projectFile.close();

    LoadingPane *loader = new LoadingPane(this, projectFile.fileName());
    emit changePane(loader);
}
