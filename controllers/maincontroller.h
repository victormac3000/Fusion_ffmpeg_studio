#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>


class MainController : public QObject
{
    Q_OBJECT

public:
    enum class Pane {
        Welcome,
        LoadProject,
        NewProject,
        About,
        Settings
    };
    Q_ENUM(Pane)

    explicit MainController(QObject *parent = nullptr);

public slots:
    void onLoadProjectClicked();
    void onNewProjectClicked();
    void onAboutClicked();
    void onSettingsClicked();

private:


signals:
    void navigateTo(MainController::Pane pane);
};

#endif // MAINCONTROLLER_H
