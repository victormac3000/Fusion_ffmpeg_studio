#include "editorpane.h"
#include "ui_editorpane.h"

EditorPane::EditorPane(QWidget *parent, Project *project) :
    QWidget(parent),
    ui(new Ui::EditorPane)
{
    ui->setupUi(this);

    this->project = project;

    // On load
    // If previews are not available -> Add work to queue and start it

    this->renderer = new Renderer;
    this->rendererThread = new QThread;

    QQuickItem* rootObject = ui->qmlWidget->rootObject();

    this->videosGridLayout = rootObject->findChild<QQuickItem*>("videosGridLayout");
    this->videoPlayer = rootObject->findChild<QQuickItem*>("videoPlayerRoot");
    this->queueGridLayout = rootObject->findChild<QQuickItem*>("queueColumnLayout");

    if (videosGridLayout == nullptr || videoPlayer == nullptr || queueGridLayout == nullptr) {
        Dialogs::critical(
            "Error loading the editor pane",
            "Could not found videosGridLayout, videoPlayerRoot or queueColumnLayout QML elements"
        );
    }

    QQuickItem* renderPreviewButton = rootObject->findChild<QQuickItem*>("renderPreviewButton");
    connect(renderPreviewButton, SIGNAL(clicked()), this, SLOT(renderPreviewClicked()), Qt::DirectConnection);

    // Connect signals
    connect(this, SIGNAL(rendererAdd(RenderWork*)), renderer, SLOT(add(RenderWork*)));
    //connect(renderer, SIGNAL(renderWorkFinished(RenderWork*,bool)), this, SLOT(renderWorkFinished(RenderWork*,bool)));

    this->renderer->moveToThread(rendererThread);
    this->rendererThread->start();


    // GUI LOAD VIDEOS
    QList<FVideo*> videosList = project->getVideos();
    int i = 0;

    for (FVideo* video: videosList) {
        QVariant returnedValue;

        bool addVideo = QMetaObject::invokeMethod(
            videosGridLayout, "addVideo",
            Q_RETURN_ARG(QVariant, returnedValue),
            Q_ARG(QVariant, video->getId()),
            Q_ARG(QVariant, video->getIdString()),
            Q_ARG(QVariant, video->getThumnail()->fileName()),
            Q_ARG(QVariant, video->getDate().toString("dd/MM/yyyy"))
        );

        if (!addVideo) {
            qWarning() << "Could add the video " << video->getIdString() << " to the preview pane";
        }

        if (!returnedValue.canConvert<QQuickItem*>()) {
            qWarning() << "Could not convert the QML object to a video QQuickItem";

        }

        QQuickItem *videoItem = qvariant_cast<QQuickItem*>(returnedValue);
        QObject *videoItemObject = dynamic_cast<QObject*>(videoItem);

        if (!videoItemObject) {
            qWarning() << "Could not convert the video QQuickItem to a video QObject";

        }

        videos.append(QPair<FVideo*, QObject*>(video, videoItemObject));

        QVariant selected = videoItemObject->property("selected");

        if (selected.isValid() && selected.toBool()) {
            this->selected = i;
        }

        i++;
    }

    if (videos.length() > 0) {
        setPlayerSource(videos.first().first);
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

void EditorPane::setPlayerSource(FVideo* video, int mode)
{
    bool setSource = false;

    if (mode == PLAYER_DEFAULT) {
        setSource = QMetaObject::invokeMethod(
            videoPlayer, "setSource", Q_ARG(QVariant, video->getEquirectangularLow()->fileName())
        );
    }

    if (!setSource) {
        qWarning() << "Could add the video " << video->getIdString() << " to the player";
    }
}

void EditorPane::renderPreviewClicked()
{
    qDebug() << "THE VIDEO TO RENDER IS ID " << videos.at(selected).first->getIdString();

    /*
    RenderWork *renderWork = new RenderWork(nullptr, project, videos.at(selected)->getVideo(), RENDER_PREVIEW);

    bool sucess = QMetaObject::invokeMethod(
        queueGridLayout, "addQueueItem",
        Q_ARG(QVariant, "RENDERING PREVIEW"),
        Q_ARG(QVariant, "x"),
        Q_ARG(QVariant, "x")
        );

    if (!sucess) {
        qWarning() << "Could add the queue ";
    }

    emit rendererAdd(renderWork);
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
