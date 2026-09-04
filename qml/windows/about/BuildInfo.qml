import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Rectangle {
    id: root

    required property var paneController
    readonly property var buildInfo: paneController.getBuildInfo()

    color: "lightgrey"

    ScrollView {
        anchors.fill: parent

        ColumnLayout {
            id: itemsLayout
            width: root.width
            height: root.height

            BuildItem {
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                name: "Compilation date"
                value: buildInfo["datetime"]
            }

            BuildItem {
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                name: "Compilation type"
                value: buildInfo["type"]
            }

            BuildItem {
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                name: "Compilation OS"
                value: buildInfo["os"]
            }

            BuildItem {
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                name: "Compilation OS Version"
                value: buildInfo["os_version"]
            }

            // Used for compacting the BuildItem components
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "transparent"
            }
        }
    }
}
