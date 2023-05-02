#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->pane = new WelcomePane(this);
    ui->paneHolder->addWidget(pane);

    // Signals for menu bar
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(displayAbout()));
    connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(displayPreferences()));

    // Signals for changing panes
    connect(this->pane, SIGNAL(changePane(QWidget*)), this, SLOT(changePane(QWidget*)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete preferencesWindow;
    delete aboutWindow;
}

bool MainWindow::changePane(QWidget *pane)
{
    if (pane == nullptr) {
        qWarning() << "Tried to change to a nullptr pane";
        return false;
    }
    clearLayout(ui->paneHolder);
    ui->paneHolder->addWidget(pane);
    connect(pane, SIGNAL(changePane(QWidget*)), this, SLOT(changePane(QWidget*)));
    this->pane = pane;
    return true;
}

void MainWindow::clearLayout(QLayout* layout, bool deleteWidgets)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (deleteWidgets)
        {
            if (QWidget* widget = item->widget())
                widget->deleteLater();
        }
        if (QLayout* childLayout = item->layout())
            clearLayout(childLayout, deleteWidgets);
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
