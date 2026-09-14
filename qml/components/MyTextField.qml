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
    property string errorText: ""
    property string warningText: ""

    state: "normal"

    onStateChanged: {
        if (state === "normal") {
            errorText = ""
            warningText = ""
        }
        if (state === "error") {
            warningText = ""
        }
        if (state === "warning") {
            errorText = ""
        }
    }

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
        id: errorToolTip
        visible: root.state === "error" && root.hovered
        text: root.errorText
        x: (root.width - width) / 2
        y: -height - 5
        enabled: false
    }

    ToolTip {
        id: warningToolTip
        visible: root.state === "warning" && root.hovered
        text: root.warningText
        x: (root.width - width) / 2
        y: -height - 5
        enabled: false
    }
}