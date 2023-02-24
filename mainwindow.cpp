#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(displayPreferences()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::displayAbout()
{

}

void MainWindow::displayPreferences()
{
    Preferences *prefWindow = new Preferences;
    prefWindow->setWindowTitle("Fusion ffmpeg studio - Preferences");
    prefWindow->setWindowModality(Qt::ApplicationModal);
    prefWindow->show();
}

