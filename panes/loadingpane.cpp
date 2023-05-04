#include "loadingpane.h"
#include "ui_loadingpane.h"

LoadingPane::LoadingPane(QWidget *parent, QString dcimPath) :
    QWidget(parent),
    ui(new Ui::LoadingPane)
{
    ui->setupUi(this);
    this->workerThread = new QThread;
    worker.moveToThread(workerThread);
    this->workerThread->start();

    // Buttons
    connect(ui->cancel_button, SIGNAL(clicked()), this, SLOT(cancelButtonPressed()));

    // Worker actions
    QMetaObject::invokeMethod(&worker, "loadDCIM", Qt::AutoConnection, Q_ARG(QString, dcimPath));
    connect(&worker, SIGNAL(loadDCIMDone(QList<FVideo*>*)), this, SLOT(loadDCIMDone(QList<FVideo*>*)));
    connect(&worker, SIGNAL(loadDCIMError(QString)), this, SLOT(loadDCIMError(QString)));
    connect(&worker, SIGNAL(loadDCIMUpdate(int,QString)), this, SLOT(loadDCIMUpdate(int,QString)));
}

LoadingPane::~LoadingPane()
{
    workerThread->quit();
    workerThread->wait();
    delete ui;
}

void LoadingPane::loadDCIMDone(QList<FVideo*>* videos)
{
    Dialogs::ok("Loaded DCIM Ok");
    emit changePane(new EditorPane(this, videos));
}

void LoadingPane::loadDCIMError(QString error)
{
    Dialogs::warning("Could not read the DCIM video structure: " + error);
    emit changePane(new WelcomePane(this));
}

void LoadingPane::loadDCIMUpdate(int percent, QString message)
{
    ui->loading_bar->setValue(percent);
    ui->loading_percent->setText(QString::number(percent) + "%");
    if (!message.isEmpty()) {
        ui->loading_text->setText(message);
    }
}

void LoadingPane::cancelButtonPressed()
{
    qDebug() << "Loading pane cancel button killing worker";
    workerThread->quit();
    workerThread->wait();
}
