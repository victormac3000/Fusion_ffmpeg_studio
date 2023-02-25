#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include <preferences.h>
#include <about.h>
#include <errorhandler.h>

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
    void openDCIMFolder();

private:
    Ui::MainWindow *ui;
    ErrorHandler *error;
};
#endif // MAINWINDOW_H
