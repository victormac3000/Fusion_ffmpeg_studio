#include "editorcontroller.h"

#include <QQmlEngine>
#include <QHttpServer>
#include <QTcpServer>

EditorController::EditorController(QObject *parent)
    : BaseController{parent}
{
    qDebug() << "EditorController thread" << QThread::currentThreadId();

    httpServer.route("/video", QHttpServerRequest::Method::Get,
     [](const QHttpServerRequest &request, QHttpServerResponder &responder)
    {
        const QString path = QUrlQuery(request.url()).queryItemValue("path", QUrl::FullyDecoded);

        auto *file = new QFile(path);
        if (!file->open(QIODevice::ReadOnly)) {
            delete file;
            responder.sendResponse(QHttpServerResponder::StatusCode::NotFound);
            return;
        }

        QHttpHeaders headers;
        headers.append("Content-Type", "video/mp4");
        headers.append("Access-Control-Allow-Origin", "*");

        responder.write(file, headers, QHttpServerResponder::StatusCode::Ok);
    });

    tcpserver = new QTcpServer();
    if (!tcpserver->listen(QHostAddress::LocalHost, 15000) || !httpServer.bind(tcpserver)) {
        qWarning() << "Could not start the internal HTTP server";
        delete tcpserver;
        return;
    }
    qDebug() << "Internal HTTP server listening on port" << tcpserver->serverPort();
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
