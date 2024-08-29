#ifndef PROJECTCREATORSD_H
#define PROJECTCREATORSD_H

#include <QWidget>

namespace Ui {
class ProjectCreatorSd;
}

class ProjectCreatorSd : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectCreatorSd(QWidget *parent = nullptr);
    ~ProjectCreatorSd();

signals:
    void changePane(QWidget *pane);

private:
    Ui::ProjectCreatorSd *ui;
};

#endif // PROJECTCREATORSD_H
