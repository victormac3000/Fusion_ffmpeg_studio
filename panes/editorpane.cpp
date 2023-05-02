#include "editorpane.h"
#include "ui_editorpane.h"

EditorPane::EditorPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorPane)
{
    ui->setupUi(this);
}

EditorPane::~EditorPane()
{
    delete ui;
}
