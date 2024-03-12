#ifndef LOADINGPANE_H
#define LOADINGPANE_H

#include <QWidget>
#include <QDir>

#include "panes/welcomepane.h"
#include "panes/editorpane.h"
#include "utils/dialogs.h"
#include "worker.h"

class MainWindow;

namespace Ui {
class LoadingPane;
}

class LoadingPane : public QWidget
{
    Q_OBJECT

public:
    explicit LoadingPane(QWidget *parent = nullptr, QString projectPath = "", QString dcimPath = "", QString projectName = "");
    ~LoadingPane();

signals:
    void changePane(QWidget *pane);

private slots:
    void loadProjectFinished(Project *project);
    void loadProjectError(QString error);
    void loadProjectUpdate(int percent, QString message);

private:
    Ui::LoadingPane *ui;
    MainWindow* mainWindow;
    Worker worker;
    QThread *workerThread;

};

#endif // LOADINGPANE_H
