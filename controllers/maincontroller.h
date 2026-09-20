#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include "models/project.h"

#include <QObject>
#include <QVariantMap>

class MainController : public QObject
{
    Q_OBJECT

public:
    enum class Pane {
        Welcome,
        NewProject,
        Loading
    };
    Q_ENUM(Pane)

    enum class Window {
        Main,
        About,
        Preferences
    };
    Q_ENUM(Window)

    explicit MainController(QObject *parent = nullptr);
    ~MainController();

    Q_INVOKABLE QString paneToUrl(Pane pane) const;
    Q_INVOKABLE QString windowToUrl(Window window) const;
    Q_INVOKABLE void stopApp();

    void navigateToM(Pane pane, const QVariantMap &attributes = {});
    void addWindowM(Window window, bool modal);
    void backM();

    Project* getProject();

public:
    Project* project = nullptr;
    QThread projectThread;

signals:
    void navigateTo(MainController::Pane pane, const QVariantMap &attributes = {});
    void addWindow(MainController::Window window, bool modal);
    void back();

    void loadProjectUpdate(LoadingProgress progress);
    void loadProjectError(LoadingError error);
};

#endif // MAINCONTROLLER_H
