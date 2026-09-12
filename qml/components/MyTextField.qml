import QtQuick
import QtQuick.Controls

import FusionFFmpegStudio

TextField {
    id: root
    padding: 5

    property color backgroundColor: "white"
    property color borderColor: "blue"
    property real borderWidth: 1
    property real radius: 2

    background: Rectangle {
        color: root.backgroundColor
        border.color: root.borderColor
        border.width: root.borderWidth
        radius: root.radius
    }
}