#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    ui->version->setText("version " + QCoreApplication::applicationVersion() + " (alpha)");
}

About::~About()
{
    delete ui;
}
