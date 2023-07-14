#ifndef FQUEUEITEM_H
#define FQUEUEITEM_H

#include "models/renderwork.h"

#include <QFrame>

namespace Ui {
class FQueueItem;
}

class FQueueItem : public QFrame
{
    Q_OBJECT

public:
    explicit FQueueItem(QWidget *parent = nullptr, RenderWork *work = nullptr);
    ~FQueueItem();

    RenderWork *getRenderWork();

private slots:
    void updateRenderStatus(FFmpegStatus *status);
    void updateRenderStatusString(QString statusString);

private:
    Ui::FQueueItem *ui;
    RenderWork *work;

};

#endif // FQUEUEITEM_H

