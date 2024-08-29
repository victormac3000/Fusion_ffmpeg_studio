#ifndef PROJECTCREATORFOLDER_H
#define PROJECTCREATORFOLDER_H

#include <QWidget>
#include <QQuickItem>

namespace Ui {
class ProjectCreatorFolder;
}

class MainWindow;

#define DEFAULT_PROJECT_GENERATOR_MAX_ITERATIONS 10000

class ProjectCreatorFolder : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectCreatorFolder(QWidget *parent = nullptr);
    ~ProjectCreatorFolder();

signals:
    void changePane(QWidget *pane);

private slots:
    void projectNameChanged();
    void browseProjectLocationClicked();
    void browseDCIMFolderClicked();
    void backButtonClicked();
    void createButtonClicked();

private:
    Ui::ProjectCreatorFolder *ui;
    MainWindow* mainWindow;
    QQuickItem *projectNameField, *projectDCIMField, *projectPathField;
    QQuickItem *linkDCIMCheckbox, *copyDCIMCheckbox;
    QQuickItem *backButton, *createProjectButton, *createProjectFolderProgressBar;
    QString rootProjectFolder;

    void generateDefaultProject();
    void generateDefaultProjectPath();
    QString generateDefaultProjectName();
};

#endif // PROJECTCREATORFOLDER_H
