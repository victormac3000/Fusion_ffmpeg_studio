#include "mainwindow.h"
#include "QtCore/qobjectdefs.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pane = MAIN_PANE;
    ui->stackedPane->setCurrentIndex(pane);
    error = new ErrorHandler(this);
    worker = new Worker(this);

    // Signals for menu bar
    connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(displayPreferences()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(displayAbout()));

    // Signals for main pane
    connect(ui->open_folder_button, SIGNAL(clicked()), this, SLOT(openDCIMFolder()));

    // Signals for worker
    connect(worker, SIGNAL(checkDCIMStructureDone(bool,QString)), this, SLOT(onCheckDCIMStructureDone(bool,QString)));
    connect(this, SIGNAL(checkDCIMStructure(QDir)), worker, SLOT(checkDCIMStructure(QDir)));
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
        this, tr("Select the DCIM folder of the fusion"), "/Users/victor/Documents/NoTM/2023_02_11_Nieve/DCIM", QFileDialog::ShowDirsOnly
    );
    if (proposedWd.isEmpty()) return;
    pane = LOADING_PANE;
    ui->stackedPane->setCurrentIndex(pane);

    dcim = QDir(proposedWd);

    emit checkDCIMStructure(dcim);
}

void MainWindow::onCheckDCIMStructureDone(bool ok, QString errorMsg)
{
    if (!ok) {
        error->warning("The gopro fusion DCIM structure is invalid: " + errorMsg);
        pane = MAIN_PANE;
        ui->stackedPane->setCurrentIndex(pane);
        return;
    }
    // Try to load the videos referenced into memory
}
