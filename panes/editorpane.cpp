#include "editorpane.h"
#include "ui_editorpane.h"

EditorPane::EditorPane(QWidget *parent, QList<FVideo*> videos) :
    QWidget(parent),
    ui(new Ui::EditorPane)
{
    ui->setupUi(this);

    this->queueLayout = new QVBoxLayout(ui->render_queue_scoll_area_widget);

    // On load
    // If previews are not available -> Add work to queue and start it

    this->renderer = new Renderer;
    this->rendererThread = new QThread;

    // Connect buttons
    connect(ui->generate_preview_button, SIGNAL(clicked()), this, SLOT(renderPreviewClicked()));
    connect(ui->render_button, SIGNAL(clicked()), this, SLOT(renderQueueClicked()));

    // Connect signals
    connect(this, SIGNAL(rendererAdd(RenderWork*)), renderer, SLOT(add(RenderWork*)));
    connect(this, SIGNAL(rendererRun()), renderer, SLOT(run()));
    connect(renderer, SIGNAL(renderWorkFinished(RenderWork*,bool)), this, SLOT(renderWorkFinished(RenderWork*,bool)));

    this->renderer->moveToThread(rendererThread);
    this->rendererThread->start();


    // GUI LOAD VIDEOS
    int i = 0;
    //int col = 0;
    for (FVideo* video: videos) {
        FVideoItem *fvideoItem = new FVideoItem(this, video);
        connect(fvideoItem, SIGNAL(clicked(FVideoItem*)), this, SLOT(videoItemClicked(FVideoItem*)));
        //if (i > 0 && i%2 == 0) col++;
        ui->videos_layout->addWidget(fvideoItem, i, 0);
        this->videos.append(fvideoItem);
        fvideoItem->show();
        i++;
    }

    if (videos.length() > 0) {
        videoItemClicked(this->videos.at(0));

        QVideoWidget *videoWidget = new QVideoWidget;
        ui->media_player_layout->addWidget(videoWidget);

        this->player = new QMediaPlayer();

        connect(ui->play_button, SIGNAL(clicked()), player, SLOT(play()));
        connect(ui->pause_button, SIGNAL(clicked()), player, SLOT(pause()));

        //player->setSource(QUrl(videos.at(0)->getSegment(0)->getBackMP4()->fileName()));
        player->setSource(QUrl(":/Images/Snow.jpg"));
        player->setVideoOutput(videoWidget);
        player->play();

    }
}

EditorPane::~EditorPane()
{
    rendererThread->quit();
    rendererThread->wait();
    delete renderer;
    delete player;
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

    videos.at(selected)->setStyleSheet("");
    selected = found;

    FVideoItem *selectedVideoItem = videos.at(selected);
    selectedVideoItem->setStyleSheet("#FVideoItem { border: 5px solid red }");

    ui->firmware_version_label->setText(QString::number(selectedVideoItem->getVideo()->getFormat().firmwareVersion, 'f', 1));
    ui->recorded_label->setText(MediaInfo::getDate(selectedVideoItem->getVideo()->getSegment(0)->getFrontMP4()).toString("dd/MM/yyyy hh:mm:ss"));
    ui->video_duration_label->setText(selectedVideoItem->getVideo()->getLength().toString("hh:mm:ss"));
}

void EditorPane::renderPreviewClicked()
{
    RenderWork *renderWork = new RenderWork(nullptr, videos.at(selected)->getVideo(), RENDER_PREVIEW);

    renderer->add(renderWork);

    FQueueItem *item = new FQueueItem(queueLayout->parentWidget(), renderWork);
    queueLayout->addWidget(item);
    queueItems.append(item);

    emit rendererRun();
}

void EditorPane::renderQueueClicked()
{
    emit rendererRun();
}

void EditorPane::renderWorkFinished(RenderWork *renderWork, bool error)
{
    if (error) {
        if (renderWork == nullptr) {
            qWarning() << "The render queue is empty";
            Dialogs::warning("The render list must have some work to do ;)");
        }
        return;
    }
}
