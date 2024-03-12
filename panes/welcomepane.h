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

#include "worker.h"
#include "utils/dialogs.h"
#include "panes/projectcreator.h"
#include "panes/loadingpane.h"
#include "windows/mainwindow.h"

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

private:
    Ui::WelcomePane *ui;
    MainWindow* mainWindow;
    QSettings settings;
    QList<QPair<RecentProject,QFrame*>> recentProjectsList;

    void loadRecentProjects();

};

#endif // WELCOMEPANE_H
