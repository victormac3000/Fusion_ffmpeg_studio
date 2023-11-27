import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Rectangle {
    width: 400
    height: 200

    RowLayout {
        anchors.centerIn: parent
        height: 50
        width: parent.width

        Item {
            Layout.fillWidth: true
        }

        Text {
            text: "Full video"
        }

        Switch {

        }

        Text {
            text: "Video segment"
        }

        Item {
            Layout.fillWidth: true
        }
    }
}
