#include "videoplayer.h"
#include "ui_videoplayer.h"

VideoPlayer::VideoPlayer(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::VideoPlayer)
{
    ui->setupUi(this);

    ui->video_quick->setSource(QUrl("qrc:/Documents/Qml/videoPlayer.qml"));
    ui->video_quick->setResizeMode(QQuickWidget::SizeRootObjectToView);
}

VideoPlayer::~VideoPlayer()
{
    delete ui;
}

void VideoPlayer::setVideo(QString videoPath)
{
    QObject *videoPlayer = ui->video_quick->rootObject()->findChild<QObject*>("videoPlayer");
    videoPlayer->setProperty("source", "file://" + videoPath);
}
