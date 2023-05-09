#include "editorpane.h"
#include "ui_editorpane.h"

EditorPane::EditorPane(QWidget *parent, QList<FVideo*> videos) :
    QWidget(parent),
    ui(new Ui::EditorPane)
{
    ui->setupUi(this);

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

        player->setSource(QUrl(videos.at(0)->getSegment(0)->getBackMP4()->fileName()));
        //player->setSource(QUrl(":/Images/Snow.jpg"));
        player->setVideoOutput(videoWidget);
        player->play();
    }
}

EditorPane::~EditorPane()
{
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
    ui->video_duration_label->setText(selectedVideoItem->getVideo()->getLength().toString("hh:mm:ss"));



}
