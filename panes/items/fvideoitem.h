#ifndef FVIDEOITEM_H
#define FVIDEOITEM_H

#include <QWidget>

#include "models/fvideo.h"
#include "utils/mediainfo.h"

namespace Ui {
class FVideoItem;
}

class FVideoItem : public QWidget
{
    Q_OBJECT

public:
    explicit FVideoItem(QWidget *parent = nullptr, FVideo *video = nullptr);
    ~FVideoItem();

private:
    Ui::FVideoItem *ui;
    FVideo *video;
};

#endif // FVIDEOITEM_H
