#ifndef EDITORCONTROLLER_H
#define EDITORCONTROLLER_H

#include "controllers/basecontroller.h"

#include <QHttpServer>

class EditorController : public BaseController
{
    Q_OBJECT
public:
    explicit EditorController(QObject *parent = nullptr);

    Q_INVOKABLE void getVideos();

private slots:
    void onVideosReady(QList<FVideo> videos);

signals:
    void videosReady(QVariantList videos);

private:
    QHttpServer httpServer;
    QTcpServer* tcpserver;

};

#endif // EDITORCONTROLLER_H
