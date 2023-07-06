#include "renderer.h"

Renderer::Renderer(QObject *parent)
    : QObject{parent}
{
    this->ffmpeg = new FFmpeg;
    this->ffmpegThread = new QThread;

    connect(ffmpeg, SIGNAL(renderDone(RenderWork*,bool)), this, SLOT(renderDone(RenderWork*,bool)));
    connect(this, SIGNAL(render(RenderWork*)), ffmpeg, SLOT(render(RenderWork*)));

    this->ffmpeg->moveToThread(ffmpegThread);
    this->ffmpegThread->start();
}

Renderer::~Renderer()
{
    ffmpegThread->quit();
    ffmpegThread->wait();
    delete ffmpegThread;
    delete ffmpeg;
}


void Renderer::add(RenderWork *renderwork)
{
    renderQueue.enqueue(renderwork);
}

void Renderer::run()
{
    if (renderQueue.isEmpty()) {
        qWarning() << "The render queue is empty";
        emit renderWorkFinished(nullptr, false);
        return;
    }
    RenderWork *work = renderQueue.dequeue();
    if (!ffmpeg->isRunning()) {
        emit render(work);
    }
}

void Renderer::renderDone(RenderWork *work, bool error)
{
    emit renderWorkFinished(work, error);
    if (!renderQueue.isEmpty()) {
        run();
    }
}
