#include "editorpane.h"
#include "ui_editorpane.h"

EditorPane::EditorPane(QWidget *parent, QList<FVideo*> *videos) :
    QWidget(parent),
    ui(new Ui::EditorPane)
{
    ui->setupUi(this);
    this->videos = videos;
}

EditorPane::~EditorPane()
{
    delete ui;
}
