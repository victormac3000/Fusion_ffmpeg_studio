import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Rectangle {
    color: "#2e2e2e"

    ColumnLayout {
        anchors.fill: parent

        Text {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 10
            text: "RENDER OPTIONS"
            color: "#fefefe"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        GridLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 10
            rows: 2
            columns: 2
            columnSpacing: 30

            Text {
                color: "#fefefe"
                text: "VIDEO QUALITY"
            }

            ComboBox {
                Layout.fillWidth: true
                model: ["1080s", "1440s", "2160s"]
            }

            Text {
                color: "#fefefe"
                text: "VIDEO CODEC"
            }

            ComboBox {
                Layout.fillWidth: true
                model: ["H264 (MP4)", "H265 (MP4)"]
            }
        }
    }
}
