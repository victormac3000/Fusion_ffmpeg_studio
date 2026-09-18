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
    property string stateText: ""
    property string warningText: ""


    onStateChanged: {
        if (state === "normal") {
            stateText = ""
        }
    }

    state: "normal"
    states: [
        State {
            name: "normal"
            PropertyChanges {
                target: root
                backgroundColor: "white"
                borderColor: "blue"
            }
        },
        State {
            name: "warning"
            PropertyChanges {
                target: root
                backgroundColor: "#FFB500"
                borderColor: "blue"
            }
        },
        State {
            name: "error"
            PropertyChanges {
                target: root
                backgroundColor: "#f5b2af"
                borderColor: "#d32f2f"
            }
        }
    ]

    background: Rectangle {
        color: root.backgroundColor
        border.color: root.borderColor
        border.width: root.borderWidth
        radius: root.radius
    }

    ToolTip {
        id: toolTip
        visible: (root.state === "error" || root.state === "warning") && root.hovered
        text: root.stateText
        x: (root.width - width) / 2
        y: -height - 5
        enabled: false
    }
}