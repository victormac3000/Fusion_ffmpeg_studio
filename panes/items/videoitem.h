#ifndef VIDEOITEMMODEL_H
#define VIDEOITEMMODEL_H


#include <QObject>
#include <QString>
#include <QQuickItem>
#include <qqml.h>

class VideoItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged)
    Q_PROPERTY(QString idString READ idString WRITE setIdString NOTIFY idStringChanged)
    Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath NOTIFY imagePathChanged)
    Q_PROPERTY(QString recorded READ recorded WRITE setRecorded NOTIFY recordedChanged)
    QML_ELEMENT

public:
    explicit VideoItem(QObject *parent = nullptr);

    bool selected();
    QString idString();
    QString imagePath();
    QString recorded();

    void setSelected(bool selected);
    void setIdString(QString idString);
    void setImagePath(QString imagePath);
    void setRecorded(QString recorded);

signals:
    void selectedChanged();
    void idStringChanged();
    void imagePathChanged();
    void recordedChanged();

private:
    bool m_selected;
    QString m_idString;
    QString m_imagePath;
    QString m_recorded;
};

#endif // VIDEOITEMMODEL_H
