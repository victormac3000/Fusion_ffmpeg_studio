import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Rectangle {    
    color: "#2e2e2e"

    RowLayout {
        anchors.fill: parent

        Item {
            Layout.fillWidth: true
        }

        Text {
            text: "Equirectangular"
            color: "#fefefe"
        }

        Switch {

        }

        Text {
            text: "Interactive 360"
            color: "#fefefe"
            rightPadding: 20
        }
    }
}
