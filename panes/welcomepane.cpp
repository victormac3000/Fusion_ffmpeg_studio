#include "welcomepane.h"
#include "ui_welcomepane.h"

WelcomePane::WelcomePane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomePane)
{
    ui->setupUi(this);

    // Buttons
    connect(ui->open_folder_button, SIGNAL(clicked()), this, SLOT(openDCIMFolder()));
}

WelcomePane::~WelcomePane()
{
    delete ui;
}

void WelcomePane::openDCIMFolder()
{
    QString proposedWd = QFileDialog::getExistingDirectory(
        this, tr("Select the DCIM folder of the fusion"), "/Users/victor/Documents/NoTM/2023_02_11_Nieve/DCIM", QFileDialog::ShowDirsOnly
    );
    if (proposedWd.isEmpty()) return;
    LoadingPane *loader = new LoadingPane(nullptr, proposedWd);
    emit changePane(loader);
}
