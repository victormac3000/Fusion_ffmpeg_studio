#include "loadingpane.h"
#include "ui_loadingpane.h"

LoadingPane::LoadingPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadingPane)
{
    ui->setupUi(this);
}

LoadingPane::~LoadingPane()
{
    delete ui;
}

void LoadingPane::updatePercent(int percent)
{
    ui->loading_bar->setValue(percent);
    ui->loading_percent->setText(QString::number(percent) + "%");
}

