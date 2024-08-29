#ifndef PROJECTCREATOR_H
#define PROJECTCREATOR_H

#include <QWidget>
#include <QSettings>
#include <QStandardPaths>

class MainWindow;

namespace Ui {
class ProjectCreator;
}

class ProjectCreator : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectCreator(QWidget *parent = nullptr);
    ~ProjectCreator();

signals:
    void changePane(QWidget *pane);

private slots:
    void sdCardClicked();
    void folderClicked();
    void backButtonClicked();

private:
    Ui::ProjectCreator *ui;
    MainWindow* mainWindow;
    QSettings settings;

};

#endif // PROJECTCREATOR_H
