#ifndef LOADINGPANE_H
#define LOADINGPANE_H

#include <QWidget>
#include <QDir>
#include <QQuickItem>

#include "models/loading.h"

class MainWindow;
class LoadinPane;
class Project;
class Worker;

namespace Ui {
class LoadingPane;
}

class LoadingPane : public QWidget
{
    Q_OBJECT

public:
    explicit LoadingPane(QWidget *parent = nullptr, LoadingInfo loadingInfo = {});
    ~LoadingPane();
    bool getInit();

signals:
    void changePane(QWidget *pane);

private slots:
    void loadProjectFinished(Project *project);
    void loadProjectError(QString error);
    void loadProjectUpdate(LoadingProgress progress);

private:
    Ui::LoadingPane *ui;
    MainWindow* mainWindow;
    Worker* workerThread;
    bool initOk = false;
    QQuickItem* progressBarGrid;
    QQuickItem* mainMessageTopText;
    QQuickItem* mainMessageLeftText;
    QQuickItem* mainMessageBar;
    QQuickItem* mainMessageRightText;
    QQuickItem* generalMessageTopText;
    QQuickItem* generalMessageLeftText;
    QQuickItem* generalMessageBar;
    QQuickItem* generalMessageRightText;
    QQuickItem* specificMessageTopText;
    QQuickItem* specificMessageLeftText;
    QQuickItem* specificMessageBar;
    QQuickItem* specificMessageRightText;

};

#endif // LOADINGPANE_H
