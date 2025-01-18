#include "editorpane.h"
#include "ui_editorpane.h"
#include "windows/mainwindow.h"
#include "models/fvideo.h"
#include "models/project.h"
#include "models/renderwork.h"
#include "utils/dialogs.h"
#include "utils/ffmpeg.h"
#include "utils/renderer.h"

EditorPane::EditorPane(QWidget* parent, Project* project) :
    QWidget(parent),
    ui(new Ui::EditorPane)
{
    ui->setupUi(this);

    this->project = project;

    MainWindow* mainWindow = (MainWindow*) parent;

    if (mainWindow == nullptr) {
        qWarning() << "MainWindow not found";
        return;
    }

    mainWindow->setWindowTitle("Editing studio");

    // On load
    // If previews are not available -> Add work to queue and start it

    this->renderer = new Renderer;
    this->rendererThread = new QThread;

    QQuickItem* rootObject = ui->qmlWidget->rootObject();

    if (rootObject == nullptr) {
        qWarning() << "Could not found quickWidget element root QML object";
        return;
    }

    this->videoPlayerSeekerBar = rootObject->findChild<QQuickItem*>("videoPlayerSeekerBar");
    this->videosGridLayout = rootObject->findChild<QQuickItem*>("videosGridLayout");
    this->videoPlayer = rootObject->findChild<QQuickItem*>("videoPlayerRoot");
    this->queueColumnLayout = rootObject->findChild<QQuickItem*>("queueColumnLayout");

    if (videosGridLayout == nullptr || videoPlayer == nullptr ||
        queueColumnLayout == nullptr || videoPlayerSeekerBar == nullptr) {
        qWarning() << "Could not found some of the QML elements";
        return;
    }

    QQuickItem* renderPreviewButton = rootObject->findChild<QQuickItem*>("renderPreviewButton");
    connect(renderPreviewButton, SIGNAL(clicked()), this, SLOT(renderPreviewClicked()), Qt::DirectConnection);

    // Connect signals
    connect(videosGridLayout, SIGNAL(activeVideoChanged(int)), this, SLOT(selectedVideoChanged(int)));

    connect(this, SIGNAL(rendererStart()), renderer, SLOT(run()));
    connect(this, SIGNAL(rendererAdd(RenderWork*)), renderer, SLOT(add(RenderWork*)));
    connect(renderer, SIGNAL(renderWorkFinished(RenderWork*,bool)), this, SLOT(renderWorkFinished(RenderWork*,bool)));

    this->renderer->moveToThread(rendererThread);
    this->rendererThread->start();


    // GUI LOAD VIDEOS
    QElapsedTimer timer;
    timer.start();

    this->videos = project->getVideos();

    for (FVideo* video: videos) {
        QVariant returnedValue;

        QFile* fThumb = video->getFrontThumbnail();
        QString fThumbPath = fThumb->fileName();

        QImageReader thumbnailReader(fThumbPath);
        thumbnailReader.setDecideFormatFromContent(true);
        QImage thumbnail = thumbnailReader.read();

        if (thumbnail.isNull()) {
            qDebug() << "Error reading thumbnail:" << thumbnailReader.errorString();
            // TODO Handle the error accordingly
        }

        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        thumbnail.save(&buffer, "JPEG");
        buffer.seek(0);

        QString thumnailBase64 = byteArray.toBase64();

        bool addVideo = QMetaObject::invokeMethod(
            videosGridLayout, "addVideo",
            Q_RETURN_ARG(QVariant, returnedValue),
            Q_ARG(QVariant, video->getId()),
            Q_ARG(QVariant, video->getIdString()),
            Q_ARG(QVariant, thumnailBase64),
            Q_ARG(QVariant, video->getDate().toString("dd/MM/yyyy"))
        );

        if (!addVideo) {
            qWarning() << "Could add the video " << video->getIdString() << " to the preview pane";
        }
    }

    qDebug() << "Load videos took " << timer.elapsed() << "ms";
}

EditorPane::~EditorPane()
{
    rendererThread->quit();
    rendererThread->wait();
    delete renderer;
    delete project;
    delete ui;
}

void EditorPane::selectedVideoChanged(int id)
{
    qDebug() << "Video changed to " << id;
    for (int i=0; i<videos.length(); i++) {
        if (videos.at(i)->getId() == id) {
            selectedVideo = i;
        }
    }
    setPlayerSource(videos.at(selectedVideo));
}

void EditorPane::setPlayerSource(FVideo* video, int mode)
{
    bool setSource = false;
    QFile* media = nullptr;

    if (mode == PLAYER_DEFAULT) {
        media = video->getEquirectangularLow();
        if (media == nullptr) {
            setSource = QMetaObject::invokeMethod(
                videoPlayer, "setDisableMessage", Q_ARG(QVariant, "The video\n has no preview")
            );
        } else {
            setSource = QMetaObject::invokeMethod(
                videoPlayer, "setSource", Q_ARG(QVariant, video->getEquirectangularLow()->fileName())
            );
        }
    }

    if (!setSource) {
        qWarning() << "Could add the video " << video->getIdString() << " to the player";
    }
}

void EditorPane::renderPreviewClicked()
{
    qDebug() << "THE VIDEO TO RENDER IS ID " << videos.at(selectedVideo)->getIdString();

    FVideo* video = videos.at(selectedVideo);
    RenderWork *renderWork = new RenderWork(nullptr, project, video, RENDER_PREVIEW);

    connect(renderWork, SIGNAL(updateRenderStatus(FFmpegStatus*)), this, SLOT(renderWorkUpdated(FFmpegStatus*)));

    bool sucess = QMetaObject::invokeMethod(
        queueColumnLayout, "addQueueItem",
        Q_ARG(QVariant, video->getIdString()),
        Q_ARG(QVariant, "RENDERING PREVIEW")
    );

    if (!sucess) {
        qWarning() << "Could add the queue ";
    }

    emit rendererAdd(renderWork);
    emit rendererStart();
}

void EditorPane::renderWorkUpdated(FFmpegStatus *status)
{
    bool sucess = QMetaObject::invokeMethod(
        queueColumnLayout, "updateQueueItemProgress",
        Q_ARG(QVariant, status->frame),
        Q_ARG(QVariant, "0"),
        Q_ARG(QVariant, status->fps),
        Q_ARG(QVariant, status->quality),
        Q_ARG(QVariant, status->size),
        Q_ARG(QVariant, status->elapsedTime),
        Q_ARG(QVariant, "00:00:00"),
        Q_ARG(QVariant, status->bitrate),
        Q_ARG(QVariant, status->speed)
    );

    if (!sucess) {
        qWarning() << "Could update the active job progress";
    }
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
    Dialogs::ok("Render was ok");
}
