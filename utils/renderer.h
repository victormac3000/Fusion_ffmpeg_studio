#ifndef RENDERER_H
#define RENDERER_H

class FFmpeg;
class RenderWork;

#include <QObject>
#include <QQueue>

class Renderer : public QObject
{
    Q_OBJECT
public:
    explicit Renderer(QObject *parent = nullptr);
    ~Renderer();

signals:
    void render(RenderWork *work);
    void renderWorkFinished(RenderWork *renderWork, bool error);

public slots:
    void add(RenderWork *renderwork);
    void run();
    void renderDone(RenderWork *work, bool error);

private:
    FFmpeg *ffmpeg;
    QThread *ffmpegThread;
    QQueue<RenderWork*> renderQueue;

};

#endif // RENDERER_H
