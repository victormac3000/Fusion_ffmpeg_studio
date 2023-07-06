#ifndef EDITORPANE_H
#define EDITORPANE_H

#include <QWidget>
#include <QMouseEvent>
#include <QVideoWidget>
#include <QQueue>

#include "QtWidgets/qboxlayout.h"
#include "models/fvideo.h"
#include "models/fsegment.h"
#include "models/renderwork.h"
#include "panes/items/fqueueitem.h"
#include "utils/dialogs.h"
#include "utils/ffmpeg.h"
#include "panes/items/fvideoitem.h"
#include "utils/renderer.h"

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

    void rendererAdd(RenderWork *renderWork);
    void rendererRun();

private slots:
    void videoItemClicked(FVideoItem *videoItem);

    void renderPreviewClicked();
    void renderQueueClicked();
    void renderWorkFinished(RenderWork *renderWork, bool error);

private:
    Ui::EditorPane *ui;
    QList<FVideoItem*> videos;
    QList<FQueueItem*> queueItems;
    QVBoxLayout *queueLayout;
    QMediaPlayer *player;
    int selected = 0;
    Renderer *renderer;
    QThread *rendererThread;
};

#endif // EDITORPANE_H
