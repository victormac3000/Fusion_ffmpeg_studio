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
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 80


            Image {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumWidth: 80
                Layout.maximumHeight: 80
                source: "Images/AppIcon.png"
            }

            ColumnLayout {
                Layout.fillHeight: true
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
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 36

            TabButton {
                Layout.fillHeight: true
                Layout.fillWidth: true
                text: qsTr("License")
            }

            TabButton {
                Layout.fillHeight: true
                Layout.fillWidth: true
                text: qsTr("Libraries")
            }

            TabButton {
                Layout.fillHeight: true
                Layout.fillWidth: true
                text: qsTr("Build info")
            }
        }

        StackLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 5
            currentIndex: aboutTabBar.currentIndex

            Flickable {
                Layout.fillHeight: true
                Layout.fillWidth: true
                clip: true
                contentHeight: height

                TextEdit {
                    id: licenseTextEdit
                    objectName: "licenseTextEdit"
                    textMargin: 5
                    anchors.fill: parent
                    anchors.leftMargin: 0
                    anchors.rightMargin: 0
                    anchors.topMargin: 0
                    anchors.bottomMargin: 0
                    font.pointSize: 20
                    font.family: "Arial"
                    readOnly: true
                    text: "This is a text"

                    Rectangle {
                        anchors.fill: parent
                        color: "white"
                        border.color: "black"
                        border.width: 3
                        z: -1
                    }

                }
            }
        }
    }

}
