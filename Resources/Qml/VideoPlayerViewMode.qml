import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Rectangle {
    width: 300
    height: 200

    RowLayout {
        anchors.centerIn: parent
        height: 50
        width: parent.width

        Item {
            Layout.fillWidth: true
        }

        Text {
            text: "Equirectangular"
        }

        Switch {

        }

        Text {
            text: "Interactive 360"
        }

        Item {
            Layout.fillWidth: true
        }

    }
}
