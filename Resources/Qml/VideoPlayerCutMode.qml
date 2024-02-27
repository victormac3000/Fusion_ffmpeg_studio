import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Rectangle {
    color: "#2e2e2e"

    RowLayout {
        anchors.fill: parent

        Text {
            text: "Full video"
            color: "#fefefe"
            leftPadding: 20
        }

        Switch {

        }

        Text {
            text: "Video segment"
            color: "#fefefe"
        }

        Item {
            Layout.fillWidth: true
        }
    }
}
