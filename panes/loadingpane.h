#ifndef LOADINGPANE_H
#define LOADINGPANE_H

#include <QWidget>
#include <QDir>

#include "panes/welcomepane.h"
#include "panes/editorpane.h"
#include "utils/dialogs.h"
#include "worker.h"

namespace Ui {
class LoadingPane;
}

class LoadingPane : public QWidget
{
    Q_OBJECT

public:
    explicit LoadingPane(QWidget *parent = nullptr, QString dcimPath = "", QString projectName = "", QString projectPath = "");
    ~LoadingPane();

signals:
    void changePane(QWidget *pane);

private slots:
    void loadProjectFinished(Project *project);
    void loadProjectError(QString error);
    void loadProjectUpdate(int percent, QString message);

private:
    Ui::LoadingPane *ui;
    QList<FVideo*> videos;
    Worker worker;
    QThread *workerThread;

};

#endif // LOADINGPANE_H
