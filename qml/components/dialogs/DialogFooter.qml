import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import FusionFFmpegStudio

ColumnLayout {
    id: root
    implicitHeight: 40
    spacing: 0

    required property var dialog
    property string type: dialog.dialogType
    property var callback

    Rectangle {
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.preferredHeight: 3
        Layout.minimumHeight: 3
        Layout.maximumHeight: 3
        color: "blue"
        border.width: 0
    }

    RowLayout {
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.margins: 15

        // Filler rectangle to push the buttons to center
        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "transparent"
        }

        DialogButton {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.maximumWidth: 50
            textPixelSize: 15
            text: qsTr("OK")
            DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
            onClicked: {
                if (callback) {
                    callback({"button": "OK"})
                }
                root.dialog.close()
            }
            visible: type != "question"
        }

        DialogButton {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.maximumWidth: 50
            textPixelSize: 15
            text: qsTr("Yes")
            DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
            onClicked: {
                if (callback) {
                    callback({"button": "YES"})
                }
                root.dialog.close()
            }
            visible: type == "question"
        }

        DialogButton {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.maximumWidth: 50
            textPixelSize: 15
            text: qsTr("No")
            DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
            onClicked: {
                if (callback) {
                    callback({"button": "NO"})
                }
                root.dialog.close()
            }
            visible: type == "question"
        }


        // Filler rectangle to push the buttons to center
        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "transparent"
        }
    }
}
