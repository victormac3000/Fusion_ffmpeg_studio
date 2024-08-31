#ifndef PROJECTCREATORSD_H
#define PROJECTCREATORSD_H

#include <QWidget>
#include <QStorageInfo>

#include "utils/myqsysinfo.h"

class MainWindow;

namespace Ui {
class ProjectCreatorSd;
}

class ProjectCreatorSd : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectCreatorSd(QWidget *parent = nullptr);
    ~ProjectCreatorSd();

    bool getInit();

signals:
    void changePane(QWidget *pane);

private:
    Ui::ProjectCreatorSd *ui;
    MainWindow* mainWindow;
    bool initOk = false;
};

#endif // PROJECTCREATORSD_H
