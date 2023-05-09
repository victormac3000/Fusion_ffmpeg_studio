#ifndef EDITORPANE_H
#define EDITORPANE_H

#include <QWidget>
#include <QMouseEvent>
#include <QVideoWidget>

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

private slots:
    void videoItemClicked(FVideoItem *videoItem);

private:
    Ui::EditorPane *ui;
    QList<FVideoItem*> videos;
    QMediaPlayer *player;
    int selected = 0;
};

#endif // EDITORPANE_H
