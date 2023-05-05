#include "editorpane.h"
#include "ui_editorpane.h"

EditorPane::EditorPane(QWidget *parent, QList<FVideo*> videos) :
    QWidget(parent),
    ui(new Ui::EditorPane)
{
    ui->setupUi(this);
    this->videos = videos;
    for (FVideo* video: videos) {
        FVideoItem *fvideoItem = new FVideoItem(this, video);
        qDebug() << "ANTES" << ui->videos_layout_frame->count();
        ui->videos_layout_frame->addWidget(fvideoItem);
        qDebug() << "DESPUES" << ui->videos_layout_frame->count();
        fvideoItem->show();
    }
}

EditorPane::~EditorPane()
{
    delete ui;
}
