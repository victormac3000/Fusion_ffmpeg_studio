#ifndef LOADINGPANE_H
#define LOADINGPANE_H

#include <QWidget>
#include <QDir>

#include "panes/editorpane.h"
#include "utils/dialogs.h"
#include "models/loadinginfo.h"
#include "worker.h"

class MainWindow;

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
    void loadProjectUpdate(int percent, QString message);

private:
    Ui::LoadingPane *ui;
    MainWindow* mainWindow;
    Worker* worker;
    QThread *workerThread;
    bool initOk = false;

};

#endif // LOADINGPANE_H
