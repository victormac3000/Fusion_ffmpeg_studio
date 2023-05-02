#ifndef LOADINGPANE_H
#define LOADINGPANE_H

#include <QWidget>
#include <QDir>

#include "worker.h"

namespace Ui {
class LoadingPane;
}

class LoadingPane : public QWidget
{
    Q_OBJECT

public:
    explicit LoadingPane(QWidget *parent = nullptr);
    ~LoadingPane();

signals:
    void changePane(QWidget *pane);

public slots:
    void updatePercent(int percent);

private slots:

private:
    Ui::LoadingPane *ui;
};

#endif // LOADINGPANE_H
