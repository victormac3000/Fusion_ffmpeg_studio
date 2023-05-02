#ifndef WELCOMEPANE_H
#define WELCOMEPANE_H

#include <QWidget>
#include <QFileDialog>

#include "worker.h"
#include "panes/loadingpane.h"
#include "utils/dialogs.h"

namespace Ui {
class WelcomePane;
}

class WelcomePane : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomePane(QWidget *parent = nullptr);
    ~WelcomePane();

signals:
    void changePane(QWidget *pane);
    void loadDCIM(QDir dcim);

private slots:
    void openDCIMFolder();
    void loadDCIMDone(QList<FVideo*> videos = QList<FVideo*>());
    void loadDCIMError(QString error);

private:
    Ui::WelcomePane *ui;
    Worker worker;
    QThread *workerThread;
};

#endif // WELCOMEPANE_H
