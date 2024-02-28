import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ProgressBar {
    Layout.fillWidth: true
    value: 0.0
    property string insideText: "Henlo"

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
        text: insideText
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        height: parent.height
        width: parent.width
        fontSizeMode: Text.Fit
        font.pointSize: 10000
        minimumPointSize: 10
    }

    Behavior on value {
        NumberAnimation {
            duration: 1000
        }
    }
}

