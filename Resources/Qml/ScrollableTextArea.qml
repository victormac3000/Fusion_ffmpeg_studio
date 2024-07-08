import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Rectangle {
    property string areaText: "Henlo"

    ColumnLayout {
        width: parent.width
        height: parent.height

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            TextArea {
                Layout.fillWidth: true
                Layout.fillHeight: true
                font.pointSize: 18
                font.family: "Arial"
                readOnly: true
                wrapMode: TextEdit.WordWrap
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignTop
                text: areaText

            }
        }
    }
}
