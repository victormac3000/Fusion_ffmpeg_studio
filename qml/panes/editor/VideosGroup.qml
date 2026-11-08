import QtQuick
import QtQuick.Layouts
import QtQuick.Shapes

import FusionFFmpegStudio
import QtQuick.Controls 2.15

Rectangle {
    id: root
    color: "grey"

    required property string dateString
    required property var toggleCallback

    RowLayout {
        id: rowLayout
        anchors.fill: parent
        spacing: 0

        Triangle {
            Layout.fillHeight: true
            Layout.fillWidth: true

            Layout.maximumHeight: rowLayout.height - 10
            Layout.maximumWidth: rowLayout.height - 10

            Layout.leftMargin: 10
            Layout.rightMargin: 5

            direction: Triangle.Down
            color: "blue"

            function toggle() {
                if (direction === Triangle.Down) {
                    direction = Triangle.Right
                    return
                }
                if (direction === Triangle.Right) {
                    direction = Triangle.Down
                    return
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    parent.toggle()
                    toggleCallback(root)
                }
            }
        }

        ToolSeparator {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.rightMargin: 5
            Layout.maximumWidth: 5
        }

        MyText {
            text: root.dateString
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
