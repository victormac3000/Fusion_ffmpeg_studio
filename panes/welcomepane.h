#ifndef WELCOMEPANE_H
#define WELCOMEPANE_H

#include <QWidget>
#include <QFileDialog>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValueRef>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QMainWindow>
#include <QFrame>
#include <QMenuBar>
#include <QQuickItem>

#include "worker.h"
#include "utils/dialogs.h"
#include "panes/projectcreator.h"
#include "panes/loadingpane.h"
#include "windows/mainwindow.h"
#include "utils/dialogs.h"

class MainWindow;

namespace Ui {
class WelcomePane;
}

struct RecentProject {
    QString name;
    QString path;
    QDateTime lastOpened;
};

class WelcomePane : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomePane(QWidget *parent = nullptr);
    ~WelcomePane();

signals:
    void changePane(QWidget *pane);

private slots:
    void searchRecentProjects(QString text);
    void openProjectButtonClicked();
    void newProjectButtonClicked();
    void recentProjectClicked(QVariant rectangle);
    void aboutButtonClicked();
    void settingsButtonClicked();

private:
    Ui::WelcomePane *ui;
    MainWindow* mainWindow;
    QSettings settings;
    QList<QPair<RecentProject,QFrame*>> recentProjectsList;
    QQuickItem* recentProjectsLayout;

    void loadRecentProjects();

};

#endif // WELCOMEPANE_H
