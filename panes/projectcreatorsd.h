#ifndef PROJECTCREATORSD_H
#define PROJECTCREATORSD_H

#include <QWidget>
#include <QStorageInfo>
#include <QQuickItem>

class MainWindow;
struct VolumeInfo;

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

private:
    Ui::ProjectCreatorSd *ui;
    MainWindow* mainWindow;
    QQuickItem *frontSDComboBox, *backSDComboBox;
    QQuickItem *frontSDRectangle, *backSDRectangle;
    QQuickItem *createProjectSDButton;
    QList<VolumeInfo> mountedVolumes;
    bool initOk = false;

    void validateSelections();
};

#endif // PROJECTCREATORSD_H
