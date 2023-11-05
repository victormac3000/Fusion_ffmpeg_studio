#ifndef FVIDEOITEM_H
#define FVIDEOITEM_H

#include <QFrame>
#include <QMouseEvent>

#include "models/fvideo.h"
#include "utils/mediainfo.h"

namespace Ui {
class FVideoItem;
}

class FVideoItem : public QFrame
{
    Q_OBJECT

public:
    explicit FVideoItem(QWidget *parent = nullptr, FVideo *video = nullptr);
    ~FVideoItem();

    FVideo *getVideo();
    void setSelected(bool selected);

signals:
    void clicked(FVideoItem *videoItem);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::FVideoItem *ui;
    FVideo *video;
    QString baseCss;
};

#endif // FVIDEOITEM_H
