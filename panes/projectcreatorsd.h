#ifndef PROJECTCREATORSD_H
#define PROJECTCREATORSD_H

#include <QWidget>
#include <QStorageInfo>
#include <QQuickItem>
#include <QFileDialog>

#define DEFAULT_PROJECT_GENERATOR_MAX_ITERATIONS 10000

class MainWindow;

#include "utils/myqsysinfo.h"

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
    void backButtonClicked();
    void checkFrontSelection(QString selectedText);
    void checkBackSelection(QString selectedText);
    void createButtonClicked();
    void projectNameChanged();
    void browseProjectLocationClicked();

private:
    Ui::ProjectCreatorSd *ui;
    MainWindow* mainWindow;
    QQuickItem *frontSDComboBox, *backSDComboBox;
    QQuickItem *frontSDRectangle, *backSDRectangle;
    QQuickItem *createProjectSDButton;
    QQuickItem* createProjectSDBackButton;
    QQuickItem* projectNameField;
    QQuickItem* projectPathField;
    QQuickItem* projectPathButton;
    QList<VolumeInfo> mountedVolumes;
    QString rootProjectFolder;
    bool initOk = false;
    VolumeInfo frontVolume, backVolume;


    void validateSelections();
    void generateDefaultProject();
    void generateDefaultProjectPath();
    QString generateDefaultProjectName();
};

#endif // PROJECTCREATORSD_H
