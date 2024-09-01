#ifndef PROJECTCREATORSD_H
#define PROJECTCREATORSD_H

#include <QWidget>
#include <QStorageInfo>
#include <QQuickItem>

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

private slots:
    void checkFrontSelection();
    void checkBackSelection();

private:
    Ui::ProjectCreatorSd *ui;
    MainWindow* mainWindow;
    QQuickItem *frontSDComboBox, *backSDComboBox;
    QQuickItem *frontSDRectangle, *backSDRectangle;
    QList<VolumeInfo> mountedVolumes;
    bool initOk = false;
};

#endif // PROJECTCREATORSD_H
