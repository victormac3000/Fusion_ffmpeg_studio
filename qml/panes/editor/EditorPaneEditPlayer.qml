import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: root
    color: "#141414"

    ColumnLayout {
        id: editPlayerLayout
        anchors.fill: parent

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "blue"
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "grey"
        }
    }
}