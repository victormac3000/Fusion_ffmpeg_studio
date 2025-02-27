#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "panes/welcomepane.h"
#include "utils/dialogs.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    WelcomePane* welcomePane = new WelcomePane(this);
    if (!welcomePane->getInit()) {
        Dialogs::criticalStart("Could not load the main window");
        return;
    }
    this->changePane(welcomePane);
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
        qCritical() << "Tried to change to a nullptr pane";
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
