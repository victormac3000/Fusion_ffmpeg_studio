#include "welcomepane.h"
#include "ui_welcomepane.h"
#include "windows/preferences.h"
#include "windows/about.h"
#include "utils/dialogs.h"
#include "utils/settings.h"
#include "panes/projectcreator.h"
#include "panes/loadingpane.h"
#include "windows/mainwindow.h"

WelcomePane::WelcomePane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomePane)
{
    ui->setupUi(this);

    this->mainWindow = (MainWindow*) parent;

    if (mainWindow == nullptr) {
        qCritical() << "MainWindow pointer is null";
        return;
    }

    mainWindow->setWindowTitle("Welcome to Fusion FFmpeg Studio");
    //mainWindow->getMenuBar()->clear();

    QQuickItem* rootObject = ui->qmlWidget->rootObject();

    if (rootObject == nullptr) {
        qCritical() << "The rootObject of welcomepane is null";
        return;
    }

    QQuickItem* appTitleLabel = rootObject->findChild<QQuickItem*>("appTitleLabel");
    QQuickItem* appVersionLabel = rootObject->findChild<QQuickItem*>("appVersionLabel");
    QQuickItem* aboutButton = rootObject->findChild<QQuickItem*>("aboutButton");
    QQuickItem* settingsButton = rootObject->findChild<QQuickItem*>("settingsButton");
    QQuickItem* loadProjectButton = rootObject->findChild<QQuickItem*>("loadProjectButton");
    QQuickItem* newProjectButton = rootObject->findChild<QQuickItem*>("newProjectButton");
    this->recentProjectsLayout = rootObject->findChild<QQuickItem*>("recentProjectsLayout");

    if (appTitleLabel == nullptr || appVersionLabel == nullptr || settingsButton == nullptr || aboutButton == nullptr ||
        loadProjectButton == nullptr || newProjectButton == nullptr || recentProjectsLayout == nullptr) {
        qCritical() << "Any of the required QML objects are null";
        return;
    }


    appTitleLabel->setProperty("text", QCoreApplication::applicationName());
    appVersionLabel->setProperty("text", QCoreApplication::applicationVersion());

    this->loadRecentProjects();


    // Buttons connections
    connect(loadProjectButton, SIGNAL(clicked()), this, SLOT(openProjectButtonClicked()));
    connect(newProjectButton, SIGNAL(clicked()), this, SLOT(newProjectButtonClicked()));
    connect(recentProjectsLayout, SIGNAL(recentProjectClicked(QVariant)), this, SLOT(recentProjectClicked(QVariant)));
    connect(settingsButton, SIGNAL(clicked()), this, SLOT(settingsButtonClicked()));
    connect(aboutButton, SIGNAL(clicked()), this, SLOT(aboutButtonClicked()));

    this->initOk = true;
}

WelcomePane::~WelcomePane()
{
    delete ui;
}

bool WelcomePane::getInit()
{
    return this->initOk;
}

void WelcomePane::openProjectButtonClicked()
{
    QString proposedProjectFile = QFileDialog::getOpenFileName(
        this, tr("Select the project file"), "/Users/victor/Documents/Untitled/project.json", tr("Fusion FFmpeg studio project (*.ffs)")
    );
    if (proposedProjectFile.isEmpty()) return;

    LoadingInfo loadingInfo;
    loadingInfo.type = LOAD_PROJECT;
    loadingInfo.projectPath = QFileInfo(proposedProjectFile).absolutePath();
    loadingInfo.rootProjectPath = QFileInfo(loadingInfo.projectPath).absolutePath();

    LoadingPane* loader = new LoadingPane(mainWindow, loadingInfo);
    if (loader->getInit()) {
        emit changePane(loader);
    } else {
        Dialogs::warning("Could not load the next menu");
    }
}

void WelcomePane::newProjectButtonClicked()
{
    ProjectCreator *creator = new ProjectCreator(mainWindow);
    if (creator->getInit()) {
        emit changePane(creator);
    } else {
        Dialogs::warning("Could not load menu");
    }
}

void WelcomePane::recentProjectClicked(QVariant rectangle)
{
    QObject *object = qvariant_cast<QObject*>(rectangle);
    if (object == nullptr) {
        qWarning() << "Could not cast the recent project clicked to a QObject";
        return;
    }
    QString projectPath = object->property("path").toString();
    if (projectPath.isEmpty()) {
        qWarning() << "The recent project clicked path is empty";
        return;
    }

    LoadingInfo loadingInfo;
    loadingInfo.type = LOAD_PROJECT;
    loadingInfo.projectPath = projectPath;
    loadingInfo.rootProjectPath = QDir(projectPath).absolutePath();

    LoadingPane* loader = new LoadingPane(mainWindow, loadingInfo);
    if (loader->getInit()) {
        emit changePane(loader);
    } else {
        Dialogs::warning("Could not load the next menu");
    }
}

void WelcomePane::aboutButtonClicked()
{
    About* aboutWindow = new About;
    aboutWindow->setWindowTitle("About");
    aboutWindow->setWindowModality(Qt::ApplicationModal);
    if (!aboutWindow->getInit()) {
        Dialogs::warning("Could not open About window");
        return;
    }
    aboutWindow->exec();
    delete aboutWindow;
}

void WelcomePane::settingsButtonClicked()
{
    Preferences* preferencesWindow = new Preferences;
    preferencesWindow->setWindowTitle("Preferences");
    preferencesWindow->setWindowModality(Qt::ApplicationModal);
    if (!preferencesWindow->getInit()) {
        Dialogs::warning("Could not open Preferences window");
        return;
    }
    preferencesWindow->exec();
    delete preferencesWindow;
}

void WelcomePane::searchRecentProjects(QString text)
{
    QSqlDatabase db = Settings::getLocalDb();

    if (!db.isValid()) {
        qWarning() << "Could not search recent projects. Invalid database";
        return;
    }

    QString sqlQuery = R"(
        SELECT name,path FROM recent_projects
        WHERE name = :name
        ORDER BY last_open DESC;
    )";

    QSqlQuery query(db);

    if (!query.prepare(sqlQuery)) {
        qWarning() << "Could not search recent projects. Query failed prepare" << query.lastError().text();
        return;
    }

    query.bindValue(":name", text);

    if (!query.exec()) {
        qWarning() << "Could not search recent projects. Query failed with error" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QString name = query.value("name").toString();
        QString path = query.value("path").toString();

        bool addRecentProject = QMetaObject::invokeMethod(
            recentProjectsLayout, "addRecentProject",
            Q_ARG(QVariant, name),
            Q_ARG(QVariant, path)
        );

        if (!addRecentProject) {
            qWarning() << "Could add the recent project "
                       << name << " to the preview pane";
        }
    }

    db.close();
}

void WelcomePane::loadRecentProjects()
{
    QSqlDatabase db = Settings::getLocalDb();

    if (!db.isValid()) {
        qWarning() << "Could not load recent projects. Invalid database";
        return;
    }

    QString sqlQuery = R"(
        SELECT name,path FROM recent_projects
        ORDER BY saved_on DESC;
    )";

    QSqlQuery query(db);

    if (!query.exec(sqlQuery)) {
        qWarning() << "Could not load recent projects. Query failed with error" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QString name = query.value("name").toString();
        QString path = query.value("path").toString();

        bool addRecentProject = QMetaObject::invokeMethod(
            recentProjectsLayout, "addRecentProject",
            Q_ARG(QVariant, name),
            Q_ARG(QVariant, path)
        );

        if (!addRecentProject) {
            qWarning() << "Could add the recent project "
                       << name << " to the preview pane";
        }
    }

    db.close();
}
