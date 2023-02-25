#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    error = new ErrorHandler(this);
    connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(displayPreferences()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(displayAbout()));
    connect(ui->open_folder_button, SIGNAL(clicked()), this, SLOT(openDCIMFolder()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::displayAbout()
{
    About *aboutWindow = new About;
    aboutWindow->setWindowTitle("About");
    aboutWindow->setWindowModality(Qt::ApplicationModal);
    aboutWindow->show();
}

void MainWindow::displayPreferences()
{
    Preferences *prefWindow = new Preferences;
    prefWindow->setWindowTitle("Preferences");
    prefWindow->setWindowModality(Qt::ApplicationModal);
    prefWindow->show();
}

void MainWindow::openDCIMFolder()
{
    QString proposedWd = QFileDialog::getExistingDirectory(
        this, tr("Select the DCIM folder of the fusion"), "", QFileDialog::ShowDirsOnly
    );

    if (proposedWd.isEmpty()) return;




    // Try to check if the structure of files is valid
}

