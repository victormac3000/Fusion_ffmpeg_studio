#include "fqueueitem.h"
#include "ui_fqueueitem.h"

FQueueItem::FQueueItem(QWidget *parent, RenderWork *work) :
    QFrame(parent),
    ui(new Ui::FQueueItem)
{
    ui->setupUi(this);
    this->work = work;

    ui->video_name->setText("VIDEO NUMBER " + work->getVideo()->getIdString());
    ui->operation->setText(work->getTypeString());

    connect(work, SIGNAL(updateRenderStatus(FFmpegStatus*)), this, SLOT(updateRenderStatus(FFmpegStatus*)));
    connect(work, SIGNAL(updateRenderStatusString(QString)), this, SLOT(updateRenderStatusString(QString)));
}

FQueueItem::~FQueueItem()
{
    delete ui;
}

void FQueueItem::updateRenderStatus(FFmpegStatus *status)
{
    ui->frame_number->setText(QString::number(status->frame));
    ui->fps->setText(QString::number(status->fps) + " fps");
    ui->quality->setText(QString::number(status->quality));
    ui->size->setText(QString::number(status->size) + " KB");
    ui->elapsed_time->setText(status->elapsedTime.toString());
    ui->bitrate->setText(QString::number(status->bitrate) + " Kbps");
    ui->speed->setText(QString::number(status->speed) + "x");
    ui->progressBar->setValue((int) status->percent*100);
    ui->progressBar_label->setText(QString::number(status->percent*100) + "%");
}

void FQueueItem::updateRenderStatusString(QString statusString)
{
    ui->status->setText(statusString);
}
