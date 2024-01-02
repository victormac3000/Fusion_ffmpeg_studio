#include "editorpane.h"
#include "ui_editorpane.h"

EditorPane::EditorPane(QWidget *parent, Project *project) :
    QWidget(parent),
    ui(new Ui::EditorPane)
{
    ui->setupUi(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    this->setLayout(mainLayout);

    qmlRegisterType<VideoItem>("es.victor.components", 1, 0, "VideoItemModel");

    QQuickWidget *qmlWidget = new QQuickWidget;
    qmlWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    qmlWidget->setSource(QUrl("qrc:/Qml/EditorPane.qml"));
    mainLayout->addWidget(qmlWidget);


    this->project = project;



    // On load
    // If previews are not available -> Add work to queue and start it

    this->renderer = new Renderer;
    this->rendererThread = new QThread;

    QQuickItem* rootObject = qmlWidget->rootObject();
    this->videosGrid = rootObject->findChild<QQuickItem*>("videosGridView");
    this->videoPlayer = rootObject->findChild<QQuickItem*>("videoPlayerRoot");

    // Connect signals
    connect(this, SIGNAL(rendererAdd(RenderWork*)), renderer, SLOT(add(RenderWork*)));
    //connect(renderer, SIGNAL(renderWorkFinished(RenderWork*,bool)), this, SLOT(renderWorkFinished(RenderWork*,bool)));

    this->renderer->moveToThread(rendererThread);
    this->rendererThread->start();


    // GUI LOAD VIDEOS

    QList<FVideo*> videos = project->getVideos();

    for (FVideo* video: videos) {
        QVariant returnValue;
        bool sucess = QMetaObject::invokeMethod(
            videosGrid, "addVideo", Q_RETURN_ARG(QVariant, returnValue)
        );
        if (sucess) {
            VideoItem *createdItem = qobject_cast<VideoItem*>(returnValue.value<QObject*>());
            createdItem->setIdString(video->getIdString());
            createdItem->setImagePath(video->getThumnail()->fileName());
            createdItem->setRecorded(video->getDate().toString("dd/MM/yyyy"));
        }
        /*
        bool sucess2 = QMetaObject::invokeMethod(
            videoPlayer, "setSource", Q_ARG(QVariant, video->getEquirectangularLow()->fileName())
        );
        if (!sucess2) {
            qWarning() << "Could not set the video preview path";
        }
        */
    }

}

EditorPane::~EditorPane()
{
    rendererThread->quit();
    rendererThread->wait();
    delete renderer;
    delete project;
    delete ui;
}

void EditorPane::renderPreviewClicked()
{
    /*
    RenderWork *renderWork = new RenderWork(nullptr, project, videos.at(selected)->getVideo(), RENDER_PREVIEW);

    emit rendererAdd(renderWork);

    FQueueItem *item = new FQueueItem(queueLayout->parentWidget(), renderWork);
    queueLayout->addWidget(item);
    queueItems.append(item);
*/
}

/*
void EditorPane::renderWorkFinished(RenderWork *renderWork, bool error)
{

    if (error) {
        if (renderWork == nullptr) {
            qWarning() << "The render queue is empty";
            Dialogs::warning("The render list must have some work to do ;)");
            return;
        }
        Dialogs::warning("Could not " + renderWork->getTypeString() + " for video " + renderWork->getVideo()->getIdString());
        return;
    }

    project->save();

    for (FQueueItem *item: queueItems) {
        if (item->getRenderWork() == renderWork) {
            queueItems.removeOne(item);
            queueLayout->removeWidget(item);
            delete item;
        }
    }

}

*/
