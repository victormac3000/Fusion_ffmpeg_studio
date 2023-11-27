import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Rectangle {
    width: 500
    height: 200

    ColumnLayout {
        anchors.centerIn: parent
        height: parent.height
        width: parent.width

        Text {
            text: "RENDER OPTIONS"
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
        }

        RowLayout {
            Layout.fillWidth: true

            Text {
                text: "Equirectangular"
            }

            Switch {

            }

            Text {
                text: "Normal"
            }

            ComboBox {
                width: 100
                model: [ "Banana", "Apple", "Coconut" ]
            }

            Item {
                Layout.fillWidth: true
            }

            Button {
                text: "Render"
            }
        }
    }


}
