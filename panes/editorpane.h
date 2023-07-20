#ifndef EDITORPANE_H
#define EDITORPANE_H

#include <QWidget>
#include <QMouseEvent>
#include <QVideoWidget>
#include <QQueue>
#include <QWebEngineView>

#include "QtWidgets/qboxlayout.h"
#include "models/fvideo.h"
#include "models/fsegment.h"
#include "models/project.h"
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
    explicit EditorPane(QWidget *parent = nullptr, Project *project = nullptr);
    ~EditorPane();

signals:
    void changePane(QWidget *pane);

    void rendererAdd(RenderWork *renderWork);

public slots:
    void saveProject();

private slots:
    void videoItemClicked(FVideoItem *videoItem);

    void renderPreviewClicked();
    void renderWorkFinished(RenderWork *renderWork, bool error);

private:
    Ui::EditorPane *ui;
    Project *project;
    QList<FVideoItem*> videos;
    QList<FQueueItem*> queueItems;
    QVBoxLayout *queueLayout;
    QWebEngineView *webEngine;
    QMediaPlayer *mediaPlayer;
    int selected = 0;
    Renderer *renderer;
    QThread *rendererThread;
};

#endif // EDITORPANE_H
