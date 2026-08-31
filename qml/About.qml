import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: root
    width: 600
    height: 400
    color: "lightblue"

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.margins: 5
            Layout.fillWidth: true
            Layout.preferredHeight: 80

            Image {
                Layout.preferredWidth: 80
                Layout.preferredHeight: 80
                source: "Images/AppIcon.png"
            }

            ColumnLayout {
                Layout.fillWidth: true

                Text {
                    Layout.fillWidth: true
                    font.pointSize: 20
                    minimumPointSize: 10
                    fontSizeMode: Text.Fit
                    font.family: "Arial"
                    text: "Fusion FFmpeg Studio"
                }

                Text {
                    id: versionText
                    objectName: "versionText"
                    Layout.fillWidth: true
                    font.pointSize: 20
                    minimumPointSize: 10
                    fontSizeMode: Text.Fit
                    font.family: "Arial"
                    text: "0.1.2"
                }

                Text {
                    Layout.fillWidth: true
                    font.pointSize: 20
                    minimumPointSize: 10
                    fontSizeMode: Text.Fit
                    font.family: "Arial"
                    text: "Made by Víctor Martínez"
                }
            }
        }

        TabBar {
            id: aboutTabBar
            Layout.fillWidth: true
            Layout.preferredHeight: 36
            Layout.margins: 5

            TabButton {
                Layout.fillWidth: true
                text: qsTr("License")
            }

            TabButton {
                Layout.fillWidth: true
                text: qsTr("Libraries")
            }

            TabButton {
                Layout.fillWidth: true
                text: qsTr("Build info")
            }
        }

        StackLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 5
            currentIndex: aboutTabBar.currentIndex

            ScrollableTextArea {
                id: licenseTextArea
                objectName: "licenseTextArea"
                Layout.fillHeight: true
            }

            Libraries {
                id: librariesInfoGrid
                objectName: "librariesInfoGrid"
                Layout.fillHeight: true
            }

            BuildInfo {
                id: buildInfoGrid
                objectName: "buildInfoGrid"
                Layout.fillHeight: true
            }
        }
    }
}
