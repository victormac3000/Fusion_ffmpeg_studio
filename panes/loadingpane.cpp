#include "loadingpane.h"
#include "panes/welcomepane.h"
#include "ui_loadingpane.h"
#include "windows/mainwindow.h"
#include "panes/editorpane.h"
#include "utils/dialogs.h"
#include "worker.h"

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

    QQuickItem* root = ui->quickWidget->rootObject();

    if (root == nullptr) {
        qWarning() << "Could not found quickWidget element root QML object.";
        return;
    }

    progressBarGrid = root->findChild<QQuickItem*>("progressBarGrid");
    mainMessageTopText = root->findChild<QQuickItem*>("mainMessageTopText");
    mainMessageLeftText = root->findChild<QQuickItem*>("mainMessageLeftText");
    mainMessageBar = root->findChild<QQuickItem*>("mainMessageBar");
    mainMessageRightText = root->findChild<QQuickItem*>("mainMessageRightText");
    generalMessageTopText = root->findChild<QQuickItem*>("generalMessageTopText");
    generalMessageLeftText = root->findChild<QQuickItem*>("generalMessageLeftText");
    generalMessageBar = root->findChild<QQuickItem*>("generalMessageBar");
    generalMessageRightText = root->findChild<QQuickItem*>("generalMessageRightText");
    specificMessageTopText = root->findChild<QQuickItem*>("specificMessageTopText");
    specificMessageLeftText = root->findChild<QQuickItem*>("specificMessageLeftText");
    specificMessageBar = root->findChild<QQuickItem*>("specificMessageBar");
    specificMessageRightText = root->findChild<QQuickItem*>("specificMessageRightText");

    if (progressBarGrid == nullptr || mainMessageTopText == nullptr ||
        mainMessageLeftText == nullptr || mainMessageBar == nullptr ||
        mainMessageRightText == nullptr || generalMessageTopText == nullptr ||
        generalMessageLeftText == nullptr || generalMessageBar == nullptr ||
        generalMessageRightText == nullptr || specificMessageTopText == nullptr ||
        specificMessageLeftText == nullptr || specificMessageBar == nullptr ||
        specificMessageRightText == nullptr) {
        qWarning() << "Could not find QML objects";
        return;
    }

    this->worker = new Worker(nullptr, loadingInfo);
    this->workerThread = new QThread;
    if (!worker->moveToThread(workerThread)) {
        qWarning() << "Could not move worker to workerThread";
        return;
    }

    connect(worker, SIGNAL(loadProjectFinished(Project*)), this, SLOT(loadProjectFinished(Project*)));
    connect(worker, SIGNAL(loadProjectError(QString)), this, SLOT(loadProjectError(QString)));
    connect(worker, SIGNAL(loadProjectUpdate(LoadingProgress)), this, SLOT(loadProjectUpdate(LoadingProgress)));
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

void LoadingPane::loadProjectUpdate(LoadingProgress progress)
{
    QString message;
    progressBarGrid->setProperty("generalMessage", false);
    progressBarGrid->setProperty("specificMessage", false);

    float mainPercent = (float) (progress.stepNumber / progress.stepCount);

    switch(progress.stepID)
    {
    case CHECK_SOURCE_FOLDERS:
        message = "Checking source folders";
        break;
    case CREATE_PROJECT_DIRS:
        message = "Creating project directories";
        break;
    case COPY_DCIM_FOLDER:
    {
        message = "Importing DCIM folder to the project";
        progressBarGrid->setProperty("generalMessage", true);
        progressBarGrid->setProperty("specificMessage", true);

        float filePercent = (float) progress.copy.currentFile.bytesDone / progress.copy.currentFile.bytesCount;

        specificMessageTopText->setProperty(
            "text",
            "Copying " + progress.copy.currentFile.name +
                " at " + QString::number(progress.copy.currentFile.speed) + "MB/s"
        );
        specificMessageBar->setProperty("insideText", QString::number(filePercent*100) + "%");
        specificMessageBar->setProperty("value", filePercent*100);

        float filesPercent = (float) progress.copy.fileNumber / progress.copy.fileCount;
        if (filePercent > 0) filesPercent *= filePercent;

        generalMessageTopText->setProperty(
            "text",
            "Copied " + QString::number(progress.copy.fileNumber) +
                " videos of " + QString::number(progress.copy.fileCount)
            );
        generalMessageBar->setProperty("insideText", QString::number(filesPercent*100) + "%");
        generalMessageBar->setProperty("value", filesPercent*100);

        mainPercent *= filesPercent;

        break;
    }
    case INDEX_VIDEOS:
    {
        message = "Checking source folders";
        progressBarGrid->setProperty("generalMessage", true);
        progressBarGrid->setProperty("specificMessage", true);


        float segmentsPercent = (float) progress.index.doneSegments / progress.index.totalSegments;

        specificMessageTopText->setProperty(
            "text",
            "Indexed " + QString::number(progress.index.doneSegments) +
                " videos of " + QString::number(progress.index.totalSegments)
            );
        specificMessageBar->setProperty("insideText", QString::number(segmentsPercent*100) + "%");
        specificMessageBar->setProperty("value", segmentsPercent*100);


        float videosPercent = (float) progress.index.doneVideos / progress.index.totalVideos;
        if (segmentsPercent > 0) videosPercent *= segmentsPercent;

        generalMessageTopText->setProperty(
            "text",
            "Indexing video " + QString::number(progress.index.doneVideos) +
                " videos of " + QString::number(progress.index.totalVideos)
        );
        generalMessageBar->setProperty("insideText", QString::number(videosPercent*100) + "%");
        generalMessageBar->setProperty("value", videosPercent*100);

        mainPercent *= segmentsPercent;

        break;
    }
    default:
        message = "Unknown step";
        break;
    }

    mainMessageTopText->setProperty("text", message);
    mainMessageBar->setProperty("insideText", QString::number(mainPercent*100) + "%");
    mainMessageBar->setProperty("value", mainPercent*100);

    /*
    #define CHECK_SOURCE_FOLDERS 0
    #define CREATE_PROJECT_DIRS 1
    #define COPY_DCIM_FOLDER 2
    #define INDEX_VIDEOS 3
    struct LoadingProgress {
        QString stepMessage;
        double stepProgress;  // 0-100
        int stepNumber;
        int stepCount;
        struct index {
            int doneVideos;
            int totalVideos;
            int doneSegments;
            int totalSegments;
        } index;
        struct copy {
            int fileCount;
            int fileNumber;
            struct currentFile {
                QString name;
                qint64 bytesCount;
                qint64 bytesDone;
                double progress;
            } currentFile;
        } copy;
    };
*/
}
