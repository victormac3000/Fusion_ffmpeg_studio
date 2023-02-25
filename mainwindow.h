#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDir>
#include <QFile>

#include <preferences.h>
#include <about.h>
#include <errorhandler.h>
#include <worker.h>

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

private slots:
    void displayAbout();
    void displayPreferences();
    void openDCIMFolder();
    void onCheckDCIMStructureDone(bool ok, QString errorMsg);

signals:
    void checkDCIMStructure(QDir dcim);

private:
    Ui::MainWindow *ui;
    ErrorHandler *error;
    Worker *worker;
    QDir dcim;
    QString errorMsg;
    int pane;
};
#endif // MAINWINDOW_H
