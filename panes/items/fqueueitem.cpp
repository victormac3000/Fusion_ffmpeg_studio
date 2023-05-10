#include "fqueueitem.h"
#include "ui_fqueueitem.h"

FQueueItem::FQueueItem(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FQueueItem)
{
    ui->setupUi(this);
}

FQueueItem::~FQueueItem()
{
    delete ui;
}
