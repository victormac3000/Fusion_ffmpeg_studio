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

                function addQueueItem(idstring, imagePath, recorded) {
                    var queueComponent = Qt.createComponent("QueueItem.qml")
                    var queueElement = queueComponent.createObject(queueColumnLayout)
                    if (activeJob == null) {
                        activeJob = queueElement
                    }
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
