#include "editorcontroller.h"

#include <QQmlEngine>
#include <QHttpServer>
#include <QTcpServer>

EditorController::EditorController(QObject *parent)
    : BaseController{parent}
{
    qDebug() << "EditorController thread" << QThread::currentThreadId();
}

void EditorController::getVideos()
{
    connect(m_appController, &MainController::videosReady,
            this, &EditorController::onVideosReady);

    QMetaObject::invokeMethod(
        this->m_appController->getProject(),
        "getVideos",
        Qt::UniqueConnection
    );
}

void EditorController::onVideosReady(QList<FVideo> videos)
{
    QVariantList videosList;

    for (FVideo& video: videos) {
        videosList.append(video.toVariantMap());
    }

    emit videosReady(videosList);
}
