import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import FusionFFmpegStudio

Rectangle {
    id: root
    color: "#000000"
    border.width: selected ? 3 : 0
    border.color: "#00A1E1"

    required property var video
    required property var canSelect
    required property var canDeselect
    property bool selected: false

    signal onDataChanged(data: var)

    onSelectedChanged: {
        if (selected) {
            border.width = 3
            image.Layout.rightMargin = 3
            image.Layout.leftMargin = 3
        } else {
            border.width = 0
            image.Layout.rightMargin = 0
            image.Layout.leftMargin = 0
        }
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            if (root.selected && canDeselect(root)) {
                root.selected = false
                return
            }
            if (!root.selected && canSelect(root)) {
                root.selected = true
                return
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 10

            MyTextField {
                Layout.fillHeight: true
                Layout.fillWidth: true

                text: qsTr("VIDEO") + " " + root.video.idString
                maximumLength: 100
                idleColor:   "#F6FBF9"
                hoverColor:  "#00A1E1"
                editingColor:"#00A1E1"
                minimumLength: 1

                onTextChanged: root.video.name = text
                onCommitted: (value) => {
                    root.video.name = value
                    onDataChanged(root.video)
                }
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            MyText {
                Layout.fillHeight: true
                elide: Text.ElideRight
                wrapMode: Text.NoWrap
                verticalAlignment: Text.AlignVCenter
                text: Qt.formatDateTime(root.video.recordedOn, "dd/MM/yyyy")
            }
        }

        Image {
            id: image
            Layout.fillHeight: true
            Layout.fillWidth: true
            fillMode: Image.Stretch
            source: "qrc:/images/Snow.jpg"
        }

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 10

            Image {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.maximumHeight: parent.height
                Layout.maximumWidth: parent.height
                Layout.rightMargin: 10
                Layout.topMargin: 0
                fillMode: Image.PreserveAspectFit
                source: "qrc:/icons/editor/picture-frame-white.png"
            }

            MyText {
                Layout.fillHeight: true
                Layout.fillWidth: true
                text: root.video.durationString
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
