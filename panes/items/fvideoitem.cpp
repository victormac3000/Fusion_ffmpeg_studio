#include "fvideoitem.h"
#include "ui_fvideoitem.h"

FVideoItem::FVideoItem(QWidget *parent, FVideo *video) :
    QFrame(parent),
    ui(new Ui::FVideoItem)
{
    ui->setupUi(this);
    this->video = video;
    ui->image->setStyleSheet("border-image: url(" + video->getSegment(0)->getBackTHM()->fileName() + ") 0 0 0 0 stretch stretch;");
    ui->name->setText("VIDEO " + video->getIdString());
    ui->date->setText(MediaInfo::getDate(video->getSegment(0)->getFrontMP4()).toString("dd/MM/yyyy"));
}

FVideoItem::~FVideoItem()
{
    delete ui;
}



FVideo *FVideoItem::getVideo()
{
    return video;
}

void FVideoItem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked(this);
    }
    QWidget::mousePressEvent(event);
}

void FVideoItem::resizeEvent(QResizeEvent *event)
{
    QSize oldSize = event->oldSize();
    QSize newSize = event->size();

    int widthDelta = newSize.width() - oldSize.width();
    if (widthDelta < 0) widthDelta *= -1;

    QSize resized(size().width(), size().height() + widthDelta);
    this->resize(resized);
    //this->adjustSize();

    QWidget::resizeEvent(event);
}
