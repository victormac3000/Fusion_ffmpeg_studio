import QtQuick 2.15
import QtQuick.Layouts

GridLayout {
    Layout.minimumHeight: 50
    Layout.maximumHeight: 50
    Layout.preferredHeight: 50
    columns: 2
    columnSpacing: 2

    property string name
    property string value

    Rectangle {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.alignment: Qt.AlignTop
        border.width: 3
        border.color: "green"

        Text {
            Layout.fillWidth: true
            font.pointSize: 20
            minimumPointSize: 10
            fontSizeMode: Text.Fit
            font.family: "Arial"
            text: name
            padding: 5
        }
    }

    Rectangle {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.alignment: Qt.AlignTop
        border.width: 3
        border.color: "green"

        Text {
            Layout.fillWidth: true
            font.pointSize: 20
            minimumPointSize: 10
            fontSizeMode: Text.Fit
            font.family: "Arial"
            text: value
            padding: 5
        }

    }
}
