#include "editorpane.h"
#include "ui_editorpane.h"

EditorPane::EditorPane(QWidget *parent, Project *project) :
    QWidget(parent),
    ui(new Ui::EditorPane)
{
    ui->setupUi(this);

    this->project = project;

    this->videoPlayer = new VideoPlayer;
    ui->video_player_frame->layout()->addWidget(this->videoPlayer);

    this->queueLayout = (QVBoxLayout*) ui->render_queue_scoll_area_widget->layout();

    // On load
    // If previews are not available -> Add work to queue and start it

    this->renderer = new Renderer;
    this->rendererThread = new QThread;

    // Connect buttons
    connect(ui->generate_preview_button, SIGNAL(clicked()), this, SLOT(renderPreviewClicked()));
    connect(ui->render_button, SIGNAL(clicked()), renderer, SLOT(run()));

    // Connect signals
    connect(this, SIGNAL(rendererAdd(RenderWork*)), renderer, SLOT(add(RenderWork*)));
    connect(renderer, SIGNAL(renderWorkFinished(RenderWork*,bool)), this, SLOT(renderWorkFinished(RenderWork*,bool)));

    this->renderer->moveToThread(rendererThread);
    this->rendererThread->start();


    // GUI LOAD VIDEOS
    QList<FVideo*> videos = project->getVideos();

    for (FVideo* video: videos) {
        // Create GUI element
        FVideoItem *fvideoItem = new FVideoItem(this, video);
        connect(fvideoItem, SIGNAL(clicked(FVideoItem*)), this, SLOT(videoItemClicked(FVideoItem*)));
        QGridLayout *layout = (QGridLayout*) ui->videos_scroll_widget->layout();
        layout->addWidget(fvideoItem);
        ui->videos_scroll_widget->layout()->addWidget(fvideoItem);
        this->videos.append(fvideoItem);
        fvideoItem->show();
    }

    ui->videos_scroll_widget->layout()->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // If there is any videos, select the first one
    if (videos.length() > 0) {
        videoItemClicked(this->videos.at(0));
    }
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

void EditorPane::videoItemClicked(FVideoItem *videoItem)
{
    int found = -1;

    for (int i=0; i<videos.length(); i++) {
        if (videoItem->getVideo()->getId() == videos.at(i)->getVideo()->getId()) {
            found = i;
            break;
        }
    }

    if (found < 0) return;

    videos.at(selected)->setSelected(false);
    selected = found;

    FVideoItem *selectedVideoItem = videos.at(selected);
    selectedVideoItem->setSelected(true);

    ui->firmware_version_label->setText(QString::number(selectedVideoItem->getVideo()->getFormat().firmwareVersion, 'f', 1));
    ui->recorded_label->setText(MediaInfo::getDate(selectedVideoItem->getVideo()->getSegment(0)->getFrontMP4()).toString("dd/MM/yyyy hh:mm:ss"));
    ui->video_duration_label->setText(selectedVideoItem->getVideo()->getLength().toString("hh:mm:ss"));

    if (selectedVideoItem->getVideo()->isEquirectangularLowValid()) {
        videoPlayer->setVideo(selectedVideoItem->getVideo()->getEquirectangularLow()->fileName());
    }
}

void EditorPane::renderPreviewClicked()
{
    RenderWork *renderWork = new RenderWork(nullptr, project, videos.at(selected)->getVideo(), RENDER_PREVIEW);

    emit rendererAdd(renderWork);

    FQueueItem *item = new FQueueItem(queueLayout->parentWidget(), renderWork);
    queueLayout->addWidget(item);
    queueItems.append(item);
}

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
