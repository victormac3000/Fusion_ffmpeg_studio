import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 15

    property string message: ""

    RowLayout {
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.margins: 15

        Label {
            Layout.fillWidth: true
            Layout.fillHeight: true
            text: root.message
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
        }
    }
}
