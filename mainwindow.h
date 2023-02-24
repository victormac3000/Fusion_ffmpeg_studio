#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "preferences.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void displayAbout();
    void displayPreferences();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
