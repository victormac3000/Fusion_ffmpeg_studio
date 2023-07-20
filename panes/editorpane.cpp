#include "editorpane.h"
#include "ui_editorpane.h"
#include "windows/mainwindow.h"

EditorPane::EditorPane(QWidget *parent, Project *project) :
    QWidget(parent),
    ui(new Ui::EditorPane)
{
    ui->setupUi(this);

    this->project = project;
    project->save();

    this->webEngine = new QWebEngineView(ui->media_player_frame);
    webEngine->load(QUrl("https://www.google.es"));
    ui->media_player_layout->addWidget(webEngine);

    this->queueLayout = new QVBoxLayout(ui->render_queue_scoll_area_widget);

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
    int i = 0;
    //int col = 0;
    QList<FVideo*> videos = project->getVideos();
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


    }
}

EditorPane::~EditorPane()
{
    rendererThread->quit();
    rendererThread->wait();
    delete renderer;
    delete webEngine;
    delete project;
    delete ui;
}

void EditorPane::saveProject()
{
    project->save();
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

    if (selectedVideoItem->getVideo()->isDualFisheyeLowValid()) {
        //ui->no_preview_widget->setVisible(false);
        //qDebug() << QUrl(selectedVideoItem->getVideo()->getDualFisheyeLow()->fileName());
        //player->setSource(QUrl(selectedVideoItem->getVideo()->getDualFisheyeLow()->fileName()));
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

    for (int i=0; i<queueLayout->count(); i++) {
        FQueueItem *item = (FQueueItem*) queueLayout->itemAt(i);
        if (item->getRenderWork() == renderWork) {
            delete item;
        }
    }
}
