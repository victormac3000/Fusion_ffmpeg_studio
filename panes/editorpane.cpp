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
    this->videosGridLayout = (QQuickItem*) ui->editor_pane_widget->rootObject()->findChild<QObject*>("videosGridLayout");

    // Connect buttons
    //connect(ui->generate_preview_button, SIGNAL(clicked()), this, SLOT(renderPreviewClicked()));
    //connect(ui->render_button, SIGNAL(clicked()), renderer, SLOT(run()));

    // Connect signals
    connect(this, SIGNAL(rendererAdd(RenderWork*)), renderer, SLOT(add(RenderWork*)));
    connect(renderer, SIGNAL(renderWorkFinished(RenderWork*,bool)), this, SLOT(renderWorkFinished(RenderWork*,bool)));

    this->renderer->moveToThread(rendererThread);
    this->rendererThread->start();


    // GUI LOAD VIDEOS

    QList<FVideo*> videos = project->getVideos();

    for (FVideo* video: videos) {
        QMetaObject::invokeMethod(videosGridLayout, "addVideoItem",
            Q_ARG(QVariant, video->getId()), Q_ARG(QVariant, video->getIdString()),
            Q_ARG(QVariant, video->getDate().toString("dd.MM.yy")),
            Q_ARG(QVariant, "Icons/VideoPlayer/no_video.png")
        );
    }


/*
    for (int i=0; i<10; i++) {
        QMetaObject::invokeMethod(queueLayout, "addQueueItem");
    }
*/
/*


    ui->videos_scroll_widget->layout()->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // If there is any videos, select the first one
    if (videos.length() > 0) {
        videoItemClicked(this->videos.at(0));
    }
*/
}

EditorPane::~EditorPane()
{
    rendererThread->quit();
    rendererThread->wait();
    delete videoPlayer;
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

void EditorPane::renderWorkFinished(RenderWork *renderWork, bool error)
{
/*
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
*/
}
