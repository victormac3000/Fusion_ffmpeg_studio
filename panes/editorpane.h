#ifndef EDITORPANE_H
#define EDITORPANE_H

#include <QWidget>

#include "models/fvideo.h"
#include "models/fsegment.h"
#include "utils/dialogs.h"
#include "panes/items/fvideoitem.h"

namespace Ui {
class EditorPane;
}

class EditorPane : public QWidget
{
    Q_OBJECT

public:
    explicit EditorPane(QWidget *parent = nullptr, QList<FVideo*> videos = QList<FVideo*>());
    ~EditorPane();

signals:
    void changePane(QWidget *pane);

private:
    Ui::EditorPane *ui;
    QList<FVideo*> videos;
};

#endif // EDITORPANE_H
