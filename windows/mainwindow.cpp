#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->changePane(new WelcomePane(this));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete preferencesWindow;
    delete aboutWindow;
}

QMenuBar *MainWindow::getMenuBar()
{
    return nullptr;
}

void MainWindow::clearMenuBar()
{
    // TODO Implement menuBar
    //ui->menubar->clear();

    /*
    if (ui->menubar->isNativeMenuBar()) {
        ui->menubar->addMenu(QCoreApplication::applicationName());
    }
    ui->menubar->addMenu("File");
    */
}

bool MainWindow::changePane(QWidget* pane)
{
    if (pane == nullptr) {
        qWarning() << "Tried to change to a nullptr pane";
        return false;
    }
    clearLayout(ui->paneHolder);
    connect(pane, SIGNAL(changePane(QWidget*)), this, SLOT(changePane(QWidget*)));
    ui->paneHolder->addWidget(pane);
    return true;
}

void MainWindow::menuBarStartDrag()
{
    dragging = true;
}

void MainWindow::menuBarEndDrag()
{

}

void MainWindow::clearLayout(QLayout* layout)
{
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        if (QLayout* childLayout = item->layout()) {
            clearLayout(childLayout);
        }
        delete item;
    }
}

void MainWindow::displayAbout()
{
    if (aboutWindow == nullptr) {
        aboutWindow = new About;
        aboutWindow->setWindowTitle("About");
        aboutWindow->setWindowModality(Qt::ApplicationModal);
    }
    aboutWindow->show();
}

void MainWindow::displayPreferences()
{
    if (preferencesWindow == nullptr) {
        preferencesWindow = new Preferences;
        preferencesWindow->setWindowTitle("Preferences");
        preferencesWindow->setWindowModality(Qt::ApplicationModal);
    }
    preferencesWindow->show();
}
