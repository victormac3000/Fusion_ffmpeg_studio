#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "panes/welcomepane.h"
#include "preferences.h"
#include "about.h"
#include "utils/dialogs.h"
#include "worker.h"

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

void MainWindow::setTitle(QString title)
{
    setWindowTitle(QCoreApplication::applicationName() + " - " + title);
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
