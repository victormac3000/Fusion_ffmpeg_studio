#include "loadingpane.h"
#include "panes/welcomepane.h"
#include "ui_loadingpane.h"
#include "windows/mainwindow.h"

LoadingPane::LoadingPane(QWidget *parent, LoadingInfo loadingInfo) :
    QWidget(parent),
    ui(new Ui::LoadingPane)
{
    ui->setupUi(this);

    this->mainWindow = (MainWindow*) parent;

    if (mainWindow == nullptr) {
        qWarning() << "MainWindow pointer is null";
        return;
    }

    mainWindow->setTitle("Loading project");

    this->worker = new Worker(nullptr, loadingInfo);
    this->workerThread = new QThread;
    if (!worker->moveToThread(workerThread)) {
        qWarning() << "Could not move worker to workerThread";
        return;
    }

    connect(worker, SIGNAL(loadProjectFinished(Project*)), this, SLOT(loadProjectFinished(Project*)));
    connect(worker, SIGNAL(loadProjectError(QString)), this, SLOT(loadProjectError(QString)));
    connect(worker, SIGNAL(loadProjectUpdate(int,QString)), this, SLOT(loadProjectUpdate(int,QString)));
    this->workerThread->start();

    QMetaObject::invokeMethod(worker, "work");

    initOk = true;
}

LoadingPane::~LoadingPane()
{
    workerThread->quit();
    workerThread->wait();
    delete workerThread;
    delete worker;
    delete ui;
}

bool LoadingPane::getInit()
{
    return initOk;
}

void LoadingPane::loadProjectFinished(Project* project)
{
    workerThread->quit();
    workerThread->wait();
    emit changePane(new EditorPane(mainWindow, project));
}

void LoadingPane::loadProjectError(QString error)
{
    workerThread->quit();
    workerThread->wait();
    Dialogs::warning("Could not load the project, see the logs for more information", "loadProjectError: " + error);
    emit changePane(new WelcomePane(mainWindow));
}

void LoadingPane::loadProjectUpdate(int percent, QString message)
{
    ui->loading_bar->setValue(percent);
    ui->loading_percent->setText(QString::number(percent) + "%");
    if (!message.isEmpty()) {
        ui->loading_text->setText(message);
    }
}
