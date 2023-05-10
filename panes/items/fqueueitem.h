#ifndef FQUEUEITEM_H
#define FQUEUEITEM_H

#include <QFrame>

namespace Ui {
class FQueueItem;
}

class FQueueItem : public QFrame
{
    Q_OBJECT

public:
    explicit FQueueItem(QWidget *parent = nullptr);
    ~FQueueItem();

private:
    Ui::FQueueItem *ui;
};

#endif // FQUEUEITEM_H
