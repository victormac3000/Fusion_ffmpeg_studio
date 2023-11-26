#ifndef EDITORPANE_H
#define EDITORPANE_H

#include <QWidget>
#include <QMouseEvent>
#include <QQueue>
#include <QVBoxLayout>
#include <QSpacerItem>

#include "models/fvideo.h"
#include "models/fsegment.h"
#include "models/project.h"
#include "models/renderwork.h"
#include "panes/items/fqueueitem.h"
#include "panes/items/videoitem.h"
#include "utils/dialogs.h"
#include "utils/ffmpeg.h"
#include "utils/renderer.h"

namespace Ui {
class EditorPane;
}

class EditorPane : public QWidget
{
    Q_OBJECT

public:
    explicit EditorPane(QWidget *parent = nullptr, Project *project = nullptr);
    ~EditorPane();

signals:
    void changePane(QWidget *pane);
    void rendererAdd(RenderWork *renderWork);


private slots:
    void renderPreviewClicked();
    void renderWorkFinished(RenderWork *renderWork, bool error);

private:
    Ui::EditorPane *ui;
    Project *project;
    QQuickItem *videosGrid, *videoPlayer;
    QList<FQueueItem*> queueItems;
    int selected = 0;
    Renderer *renderer;
    QThread *rendererThread;
};

#endif // EDITORPANE_H
