import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: root
    color: "#141414"

    RowLayout {
        id: renderLayout
        anchors.fill: parent
        anchors.margins: 10

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "red"
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "yellow"
        }
    }
}