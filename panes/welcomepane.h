#ifndef WELCOMEPANE_H
#define WELCOMEPANE_H

#include <QWidget>
#include <QFileDialog>

#include "worker.h"
#include "panes/loadingpane.h"
#include "utils/dialogs.h"

namespace Ui {
class WelcomePane;
}

class WelcomePane : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomePane(QWidget *parent = nullptr);
    ~WelcomePane();

signals:
    void changePane(QWidget *pane);

private slots:
    void openDCIMFolder();

private:
    Ui::WelcomePane *ui;

};

#endif // WELCOMEPANE_H
