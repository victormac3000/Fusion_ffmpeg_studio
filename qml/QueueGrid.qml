import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Rectangle {
    id: queueRectangle
    objectName: "queueRectangle"
    color: "#2e2e2e"

    ColumnLayout {
        anchors.fill: parent

        Flickable {
            Layout.fillWidth: true
            Layout.fillHeight: true
            flickableDirection: Flickable.VerticalFlick
            boundsBehavior: Flickable.DragOverBounds
            clip: true
            contentHeight: queueColumnLayout.height

            ColumnLayout {
                id: queueColumnLayout
                objectName: "queueColumnLayout"
                width: parent.width

                property QueueItem activeJob: null

                Component.onCompleted: {
                /*
                    for (var i=0; i<100; i++) {
                        addQueueItem("", "", "")
                    }
                */
                }

                function addQueueItem(idstring, operation) {
                    var queueComponent = Qt.createComponent("QueueItem.qml")
                    var queueElement = queueComponent.createObject(queueColumnLayout)
                    queueElement.videoIdString = idstring
                    queueElement.operation = operation
                    if (activeJob == null) {
                        activeJob = queueElement
                    }
                }

                function updateQueueItemProgress(currentFrame, totalFrames, fps, quality, size, eTime, rTime, bitrate, speed) {
                    activeJob.currentFrame = currentFrame
                    activeJob.totalFrames = totalFrames
                    activeJob.fps = fps
                    activeJob.quality = quality
                    activeJob.size = size
                    activeJob.elapsedTime = eTime
                    activeJob.remainingTime = rTime
                    activeJob.bitrate = bitrate
                    activeJob.speed = speed
                }

                function queueItemClicked(queueItem, action) {

                }

                function getActiveJob() {
                    return activeJob
                }
            }

            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AlwaysOn
            }
        }

    }
}
