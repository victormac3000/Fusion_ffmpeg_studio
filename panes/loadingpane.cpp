#include "loadingpane.h"
#include "ui_loadingpane.h"
#include "windows/mainwindow.h"

LoadingPane::LoadingPane(QWidget *parent, QString dcimPath, QString projectName, QString projectPath) :
    QWidget(parent),
    ui(new Ui::LoadingPane)
{
    ui->setupUi(this);

    this->workerThread = new QThread;
    worker.moveToThread(workerThread);
    this->workerThread->start();

    // Worker actions
    if (dcimPath.isEmpty() || projectName.isEmpty()) {
        QMetaObject::invokeMethod(&worker, "loadProject", Qt::AutoConnection, Q_ARG(QString, projectPath));
    } else {
        QMetaObject::invokeMethod(&worker, "createProject", Qt::AutoConnection, Q_ARG(QString, dcimPath), Q_ARG(QString, projectName), Q_ARG(QString, projectPath));
    }
    connect(&worker, SIGNAL(loadProjectFinished(Project*)), this, SLOT(loadProjectFinished(Project*)));
    connect(&worker, SIGNAL(loadProjectError(QString)), this, SLOT(loadProjectError(QString)));
    connect(&worker, SIGNAL(loadProjectUpdate(int,QString)), this, SLOT(loadProjectUpdate(int,QString)));
}

LoadingPane::~LoadingPane()
{
    workerThread->quit();
    workerThread->wait();
    for (FVideo* vid: videos) delete vid;
    delete workerThread;
    delete ui;
}

void LoadingPane::loadProjectFinished(Project* project)
{
    Dialogs::ok("Loaded project Ok");
    emit changePane(new EditorPane((MainWindow*) parent(), project));
}

void LoadingPane::loadProjectError(QString error)
{
    workerThread->quit();
    workerThread->wait();
    Dialogs::warning("Could not load the project, see the logs for more information", "loadProjectError: " + error);
    emit changePane((QWidget*) new WelcomePane((QWidget*) this->parent()));
}

void LoadingPane::loadProjectUpdate(int percent, QString message)
{
    ui->loading_bar->setValue(percent);
    ui->loading_percent->setText(QString::number(percent) + "%");
    if (!message.isEmpty()) {
        ui->loading_text->setText(message);
    }
}
