#include "projectcreatorsd.h"
#include "ui_projectcreatorsd.h"

ProjectCreatorSd::ProjectCreatorSd(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProjectCreatorSd)
{
    ui->setupUi(this);
}

ProjectCreatorSd::~ProjectCreatorSd()
{
    delete ui;
}
