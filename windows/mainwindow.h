#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QMetaObject>

#include "panes/welcomepane.h"
#include "preferences.h"
#include "about.h"
#include "utils/dialogs.h"
#include "worker.h"

#define MAIN_PANE 0
#define LOADING_PANE 1


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QMenuBar* getMenuBar();
    void clearMenuBar();

signals:


private slots:
    void displayAbout();
    void displayPreferences();
    bool changePane(QWidget *pane);

private:
    Ui::MainWindow *ui;
    About *aboutWindow = nullptr;
    Preferences *preferencesWindow = nullptr;

    void clearLayout(QLayout* layout);
};
#endif // MAINWINDOW_H
