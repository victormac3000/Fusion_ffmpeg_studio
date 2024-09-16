#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QMetaObject>
#include <QMouseEvent>

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
    void setTitle(QString title);

public slots:
    bool changePane(QWidget *pane);

private slots:
    void menuBarStartDrag();
    void menuBarEndDrag();

private:
    Ui::MainWindow *ui;
    bool dragging = false;

    void clearLayout(QLayout* layout);
};
#endif // MAINWINDOW_H
