#include "loadingpane.h"
#include "ui_loadingpane.h"
#include "windows/mainwindow.h"

LoadingPane::LoadingPane(QWidget *parent, QString projectPath, QString dcimPath, QString projectName) :
    QWidget(parent),
    ui(new Ui::LoadingPane)
{
    ui->setupUi(this);

    this->mainWindowWidget = parent;
    MainWindow *mainWindow = (MainWindow*) parent;
    mainWindow->clearMenuBar();
    mainWindow->setWindowTitle(QCoreApplication::applicationName() + " - Loading project");

    this->workerThread = new QThread;
    worker.moveToThread(workerThread);
    connect(&worker, SIGNAL(loadProjectFinished(Project*)), this, SLOT(loadProjectFinished(Project*)));
    connect(&worker, SIGNAL(loadProjectError(QString)), this, SLOT(loadProjectError(QString)));
    connect(&worker, SIGNAL(loadProjectUpdate(int,QString)), this, SLOT(loadProjectUpdate(int,QString)));
    this->workerThread->start();

    // Worker actions
    if (dcimPath.isEmpty() || projectName.isEmpty()) {
        QMetaObject::invokeMethod(&worker, "loadProject", Qt::AutoConnection, Q_ARG(QString, projectPath));
    } else {
        QMetaObject::invokeMethod(&worker, "createProject", Qt::AutoConnection, Q_ARG(QString, dcimPath), Q_ARG(QString, projectName), Q_ARG(QString, projectPath));
    }
}

LoadingPane::~LoadingPane()
{
    workerThread->quit();
    workerThread->wait();
    delete workerThread;
    delete ui;
}

void LoadingPane::loadProjectFinished(Project* project)
{
    workerThread->quit();
    workerThread->wait();
    emit changePane(new EditorPane(mainWindowWidget, project));
}

void LoadingPane::loadProjectError(QString error)
{
    workerThread->quit();
    workerThread->wait();
    Dialogs::warning("Could not load the project, see the logs for more information", "loadProjectError: " + error);
    emit changePane(new WelcomePane(mainWindowWidget));
}

void LoadingPane::loadProjectUpdate(int percent, QString message)
{
    ui->loading_bar->setValue(percent);
    ui->loading_percent->setText(QString::number(percent) + "%");
    if (!message.isEmpty()) {
        ui->loading_text->setText(message);
    }
}
