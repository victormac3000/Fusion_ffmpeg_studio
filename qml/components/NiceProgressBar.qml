import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ProgressBar {
    id: root
    value: 0.0
    from: 0.0
    to: 1.0
    property string insideText: ""
    property string hoverText: ""

    function setParam(key, valueToSet) {
        if (key === "from") {
            from = valueToSet
        }
        if (key === "value") {
            value = valueToSet
        }
        if (key === "to") {
            to = valueToSet
        }
        if (key === "text") {
            insideText = valueToSet
        }
        console.log(value)
    }

    Label {
        z: 1
        text: insideText
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        height: parent.height
        width: parent.width
        fontSizeMode: Text.Fit
        font.pointSize: 10000
        minimumPointSize: 10
    }

    ToolTip {
        id: tooltip
        visible: root.hovered && hoverText.length > 0
        text: root.hoverText
        x: (root.width - width) / 2
        y: -height - 5
        enabled: false
    }

    Behavior on value {
        NumberAnimation {
            duration: 100
        }
    }
}

