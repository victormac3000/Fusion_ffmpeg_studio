#ifndef EDITORVIEWERCONTROLLER_H
#define EDITORVIEWERCONTROLLER_H

#include <QObject>
#include <QVariantMap>

class EditorViewerController : public QObject
{
    Q_OBJECT

public:
    explicit EditorViewerController(QObject *parent = nullptr);

public slots:
    void ready(const QVariantMap &data);
    void playingChanged(bool playing);
    void cameraChanged(const QVariantMap &data);

signals:
    void viewerReady(const QVariantMap &data);
    void playingChangedSignal(bool playing);
    void cameraChangedSignal(const QVariantMap &data);
};

#endif // EDITORVIEWERCONTROLLER_H
