#include "welcomepane.h"
#include "ui_welcomepane.h"

WelcomePane::WelcomePane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomePane)
{
    ui->setupUi(this);

    this->mainWindow = (MainWindow*) parent;

    if (mainWindow == nullptr) {
        Dialogs::critical(
            "Error loading the application",
            "MainWindow pointer is null"
        );
        return;
    }

    mainWindow->setWindowTitle("Welcome to Fusion FFmpeg Studio");
    //mainWindow->getMenuBar()->clear();

    QQuickItem* rootObject = ui->qmlWidget->rootObject();

    if (rootObject == nullptr) {
        Dialogs::critical(
            "Error loading the application",
            "The rootObject of welcomepane is null"
        );
        return;
    }

    QQuickItem* appTitleLabel = rootObject->findChild<QQuickItem*>("appTitleLabel");
    QQuickItem* appVersionLabel = rootObject->findChild<QQuickItem*>("appVersionLabel");
    QQuickItem* settingsButton = rootObject->findChild<QQuickItem*>("settingsButton");
    QQuickItem* loadProjectButton = rootObject->findChild<QQuickItem*>("loadProjectButton");
    QQuickItem* newProjectButton = rootObject->findChild<QQuickItem*>("newProjectButton");
    this->recentProjectsLayout = rootObject->findChild<QQuickItem*>("recentProjectsLayout");

    if (appTitleLabel == nullptr || appVersionLabel == nullptr || settingsButton == nullptr ||
        loadProjectButton == nullptr || newProjectButton == nullptr || recentProjectsLayout == nullptr) {
        Dialogs::critical(
            "Error loading the application",
            "Any of the required QML objects are null"
        );
        return;
    }


    appTitleLabel->setProperty("text", QCoreApplication::applicationName());
    appVersionLabel->setProperty("text", QCoreApplication::applicationVersion());

    this->loadRecentProjects();


    // Buttons connections
    connect(loadProjectButton, SIGNAL(clicked()), this, SLOT(openProjectButtonClicked()));
    connect(newProjectButton, SIGNAL(clicked()), this, SLOT(newProjectButtonClicked()));
    connect(recentProjectsLayout, SIGNAL(recentProjectClicked(QVariant)), this, SLOT(recentProjectClicked(QVariant)));
}

WelcomePane::~WelcomePane()
{
    delete ui;
}

void WelcomePane::openProjectButtonClicked()
{
    QString proposedProjectFile = QFileDialog::getOpenFileName(
        this, tr("Select the project file"), QSettings().value("defaultProjectPath").toString() + "/Mi proyecto", tr("Fusion FFmpeg studio project (*.ffs)")
    );
    if (proposedProjectFile.isEmpty()) return;
    LoadingPane *loader = new LoadingPane(mainWindow, QFileInfo(proposedProjectFile).absolutePath());
    emit changePane(loader);
}

void WelcomePane::newProjectButtonClicked()
{
    ProjectCreator *creator = new ProjectCreator(mainWindow);
    emit changePane(creator);
}

void WelcomePane::recentProjectClicked(QVariant rectangle)
{
    QObject *object = qvariant_cast<QObject*>(rectangle);
    if (object == nullptr) {
        qWarning() << "Could not cast the recent project clicked to a QObject";
        return;
    }
    QString path = object->property("path").toString();
    if (path.isEmpty()) {
        qWarning() << "The recent project clicked path is empty";
        return;
    }
    LoadingPane *loader = new LoadingPane(mainWindow, path);
    emit changePane(loader);
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
    qDebug() << settings.value("appData").toString() + "/recent_projects.json";

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
                        QDateTime lastOpened = QDateTime::fromString(obj.value("last_opened").toString(), Qt::RFC2822Date);
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
        bool addRecentProject = QMetaObject::invokeMethod(
            recentProjectsLayout, "addRecentProject",
            Q_ARG(QVariant, rp.name),
            Q_ARG(QVariant, rp.path)
        );

        if (!addRecentProject) {
            qWarning() << "Could add the recent project " << rp.name << " to the preview pane";
        }
    }
}
