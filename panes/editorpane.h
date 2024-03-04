#ifndef EDITORPANE_H
#define EDITORPANE_H

#include <QWidget>
#include <QQuickWidget>
#include <QMouseEvent>
#include <QQueue>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QQuickItem>

#include "models/fvideo.h"
#include "models/fsegment.h"
#include "models/project.h"
#include "models/renderwork.h"
#include "utils/dialogs.h"
#include "utils/ffmpeg.h"
#include "utils/renderer.h"

#define PLAYER_DEFAULT 0
#define PLAYER_EQUIRECTANGULAR 1
#define PLAYER_OVERCAPTURE 2

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
    void rendererStart();

public slots:


private slots:
    void selectedVideoChanged(int id);
    void renderPreviewClicked();
    void renderWorkUpdated(FFmpegStatus* status);
    void renderWorkFinished(RenderWork *renderWork, bool error);

private:
    Ui::EditorPane *ui;
    Project *project;
    QQuickItem *videosGridLayout, *videoPlayer, *queueGridLayout;
    QList<FVideo*> videos;
    int selectedVideo = 0;
    int activeQueueItem = 0;
    Renderer *renderer;
    QThread *rendererThread;

    void setPlayerSource(FVideo* video, int mode = PLAYER_DEFAULT);
};

#endif // EDITORPANE_H
