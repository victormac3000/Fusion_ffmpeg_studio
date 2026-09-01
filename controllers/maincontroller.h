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
        NewProjectFolder,
        NewProjectSd
    };
    Q_ENUM(Pane)

    enum class Window {
        Main,
        About,
        Preferences
    };
    Q_ENUM(Window)

    explicit MainController(QObject *parent = nullptr);

    Q_INVOKABLE QString paneToUrl(Pane pane) const;
    Q_INVOKABLE QString windowToUrl(Window window) const;

    void navigateToM(Pane pane);
    void addWindowM(Window window, bool modal);
    void backM();

private:


signals:
    void navigateTo(MainController::Pane pane);
    void addWindow(MainController::Window window, bool modal);
    void back();
};

#endif // MAINCONTROLLER_H
