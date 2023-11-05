#include "welcomepane.h"
#include "ui_welcomepane.h"

WelcomePane::WelcomePane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomePane)
{ 
    ui->setupUi(this);

    this->mainWindowWidget = parentWidget();
    MainWindow *mainWindow = (MainWindow*) parentWidget();
    mainWindow->getMenuBar()->clear();
    mainWindow->setWindowTitle("Welcome to Fusion FFmpeg Studio");

    this->loadRecentProjects();

    ui->app_name->setText(QCoreApplication::applicationName());
    ui->app_version->setText(QCoreApplication::applicationVersion());

    // Buttons connections
    connect(ui->search_box_projects, SIGNAL(textChanged(QString)), this, SLOT(searchRecentProjects(QString)));
    connect(ui->open_folder_button, SIGNAL(clicked()), this, SLOT(openProjectButtonClicked()));
    connect(ui->new_project_button, SIGNAL(clicked(bool)), this, SLOT(newProjectButtonClicked()));
}

WelcomePane::~WelcomePane()
{
    delete ui;
}

void WelcomePane::openProjectButtonClicked()
{
    QString proposedProjectFile = QFileDialog::getOpenFileName(
        this, tr("Select the project file"), QSettings().value("defaultProjectPath").toString() + "/MiProyecto", tr("Fusion FFmpeg studio project (*.ffs)")
    );
    if (proposedProjectFile.isEmpty()) return;
    LoadingPane *loader = new LoadingPane(mainWindowWidget, QFileInfo(proposedProjectFile).absolutePath());
    emit changePane(loader);
}

void WelcomePane::newProjectButtonClicked()
{
    ProjectCreator *creator = new ProjectCreator(mainWindowWidget);
    emit changePane(creator);
}

void WelcomePane::searchRecentProjects(QString text)
{
    for (const QPair<RecentProject, QFrame*> &pair: recentProjectsList) {
        bool contains = text.isEmpty() ? true : pair.first.name.contains(text, Qt::CaseInsensitive);
        pair.second->setVisible(contains);
    }
}

void WelcomePane::loadRecentProjects()
{
    QWidget *recentProjectsWidget = new QWidget(this);
    QVBoxLayout *recentProjectsLayout = new QVBoxLayout(recentProjectsWidget);
    ui->recent_projects->setWidget(recentProjectsWidget);
    ui->recent_projects->setWidgetResizable(true);


    // Check if recent projects exists
    QFile recentProjectsFile = QFile(settings.value("appData").toString() + "/recent_projects.json");
    QList<RecentProject> recentProjects;

    if (recentProjectsFile.exists() && recentProjectsFile.open(QFile::ReadOnly)) {
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(recentProjectsFile.readAll(), &error);
        if (error.error == QJsonParseError::NoError && doc.isArray()) {
            QJsonArray mArray = doc.array();
            for (QJsonValueRef valueRef: mArray) {
                if (valueRef.isObject()) {
                    QJsonObject obj = valueRef.toObject();
                    if (obj.contains("name") && obj.contains("path") && obj.contains("last_opened")) {
                        QString name = obj.value("name").toString();
                        QString path = obj.value("path").toString();
                        QDateTime lastOpened = QDateTime::fromString(obj.value("last_opened").toString(), Qt::ISODate);
                        if (QFile::exists(path)) {
                            RecentProject rp;
                            rp.name = name;
                            rp.path = path;
                            rp.lastOpened = lastOpened;
                            recentProjects.append(rp);
                        }
                    }
                }
            }
        }
    }

    // Order recent projects. The most recent should be at the top. Plain simple bubble sort.
    bool swapped;
    for (int i=0; i<recentProjects.size()-1; i++) {
        swapped = false;
        for (int j=0; j<recentProjects.size()-i-1; j++) {
            if (recentProjects.at(j).lastOpened < recentProjects.at(j+1).lastOpened) {
                recentProjects.swapItemsAt(j, j+1);
                swapped = true;
            }
        }
        if (swapped == false) break;
    }

    for (const RecentProject &rp: recentProjects) {
        QFrame *frame = new QFrame;
        frame->setLayout(new QVBoxLayout(frame));
        QLabel *projectName = new QLabel(rp.name);
        QFont projectNameFont;
        projectNameFont.setBold(true);
        projectName->setFont(projectNameFont);
        QLabel *projectPath = new QLabel(rp.path);
        frame->layout()->addWidget(projectName);
        frame->layout()->addWidget(projectPath);
        frame->setStyleSheet("QFrame { background-color: rgb(43,45,46) } QFrame::hover,QLabel::hover { background-color: rgb(61,65,67) }");
        frame->setMaximumHeight(60);
        recentProjectsLayout->addWidget(frame);
        recentProjectsList.append(QPair<RecentProject,QFrame*>(rp, frame));
    }

    recentProjectsLayout->addSpacerItem(new QSpacerItem(recentProjectsWidget->width(), 200, QSizePolicy::Maximum, QSizePolicy::Maximum));

}
