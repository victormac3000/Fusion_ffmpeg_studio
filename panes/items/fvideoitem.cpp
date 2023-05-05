#include "fvideoitem.h"
#include "ui_fvideoitem.h"

FVideoItem::FVideoItem(QWidget *parent, FVideo *video) :
    QWidget(parent),
    ui(new Ui::FVideoItem)
{
    ui->setupUi(this);
    this->video = video;
    ui->image->setStyleSheet("border-image: url(" + video->getSegment(0)->getBackTHM()->fileName() + ") 0 0 0 0 stretch stretch;");
    ui->name->setText("VIDEO " + video->getSegment(0)->getIdString());
    ui->date->setText(MediaInfo::getDate(video->getSegment(0)->getFrontMP4()).toString("dd/MM/yyyy"));
}

FVideoItem::~FVideoItem()
{
    delete ui;
}
