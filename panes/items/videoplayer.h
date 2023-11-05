#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QFrame>
#include <QMediaPlayer>
#include <QWebEngineView>
#include <QVideoWidget>
#include <QQuickWidget>
#include <QtWidgets>
#include <QtMultimedia>
#include <QtQuickWidgets>

namespace Ui {
class VideoPlayer;
}

class VideoPlayer : public QFrame
{
    Q_OBJECT

public:
    explicit VideoPlayer(QWidget *parent = nullptr);
    ~VideoPlayer();

    void setVideo(QString videoPath);

private:
    Ui::VideoPlayer *ui;

};

#endif // VIDEOPLAYER_H
