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
#include <QSettings>

class MainWindow;

namespace Ui {
class WelcomePane;
}

class WelcomePane : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomePane(QWidget *parent = nullptr);
    ~WelcomePane();

    bool getInit();

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
    bool initOk;
    QSettings settings;
    QQuickItem* recentProjectsLayout;

    void loadRecentProjects();

};

#endif // WELCOMEPANE_H
