#include "editorviewercontroller.h"

EditorViewerController::EditorViewerController(QObject *parent)
    : QObject(parent)
{}

void EditorViewerController::ready(
    const QVariantMap &data)
{
    emit viewerReady(data);
}

void EditorViewerController::playingChanged(
    bool playing)
{
    emit playingChangedSignal(playing);
}

void EditorViewerController::cameraChanged(
    const QVariantMap &data)
{
    emit cameraChangedSignal(data);
}