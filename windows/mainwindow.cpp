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

QMenuBar *MainWindow::getMenuBar()
{
    return ui->menubar;
}

void MainWindow::clearMenuBar()
{
    ui->menubar->clear();
    if (ui->menubar->isNativeMenuBar()) {
        ui->menubar->addMenu(QCoreApplication::applicationName());
    }
    ui->menubar->addMenu("File");
}

bool MainWindow::changePane(QWidget *pane)
{
    if (pane == nullptr) {
        qWarning() << "Tried to change to a nullptr pane";
        return false;
    }
    clearLayout(ui->paneHolder);
    connect(pane, SIGNAL(changePane(QWidget*)), this, SLOT(changePane(QWidget*)));
    ui->paneHolder->addWidget(pane);
    this->pane = pane;
    QString paneName = pane->metaObject()->className();
    if (paneName == "EditorPane") {
        connect(ui->save_project, SIGNAL(triggered()), pane, SLOT(saveProject()));
    }
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
