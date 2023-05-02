#include "welcomepane.h"
#include "ui_welcomepane.h"

WelcomePane::WelcomePane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomePane)
{
    ui->setupUi(this);

    this->workerThread = new QThread;

    connect(ui->open_folder_button, SIGNAL(clicked()), this, SLOT(openDCIMFolder()));
    connect(this, SIGNAL(loadDCIM(QDir)), &worker, SLOT(loadDCIM(QDir)));
    connect(&worker, SIGNAL(loadDCIMDone(QList<FVideo*>)), this, SLOT(loadDCIMDone(QList<FVideo*>)));
    connect(&worker, SIGNAL(loadDCIMError(QString)), this, SLOT(loadDCIMError(QString)));

    worker.moveToThread(workerThread);
    workerThread->start();
}

WelcomePane::~WelcomePane()
{
    workerThread->quit();
    workerThread->wait();
    delete ui;
}

void WelcomePane::openDCIMFolder()
{
    QString proposedWd = QFileDialog::getExistingDirectory(
        this, tr("Select the DCIM folder of the fusion"), "/Users/victor/Documents/NoTM/2023_02_11_Nieve/DCIM", QFileDialog::ShowDirsOnly
    );
    if (proposedWd.isEmpty()) return;

    LoadingPane *loader = new LoadingPane;
    connect(&worker, SIGNAL(loadDCIMUpdate(int)), loader, SLOT(updatePercent(int)));
    emit changePane(loader);
    emit loadDCIM(QDir(proposedWd));
}

void WelcomePane::loadDCIMDone(QList<FVideo*> videos)
{
    Dialogs::ok("Loaded DCIM Ok");
    for (FVideo *video: videos) {
        qDebug() << "START VIDEO";
        qDebug() << video->toString();
        qDebug() << "END VIDEO";
    }
}

void WelcomePane::loadDCIMError(QString error)
{
    Dialogs::warning("Could not read the DCIM video structure: " + error);
}
