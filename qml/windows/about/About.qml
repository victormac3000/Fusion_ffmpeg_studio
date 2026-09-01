import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import FusionFFmpegStudio

Window {
    id: root
    width: 600
    height: 400

    color: "lightblue"

    property var parentWindow

    maximumHeight: parentWindow.height * 0.9
    maximumWidth: parentWindow.width * 0.9

    minimumHeight: parentWindow.minimumHeight * 0.9
    minimumWidth: parentWindow.minimumWidth * 0.9

    AboutController {
        id: controller
        appController: mainController
    }

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
                source: "qrc:/images/AppIcon.png"
            }

            ColumnLayout {
                Layout.fillWidth: true

                RowLayout {
                    Layout.fillWidth: true
                    Text {
                        Layout.fillWidth: true
                        font.pointSize: 20
                        minimumPointSize: 10
                        fontSizeMode: Text.Fit
                        font.family: "Arial"
                        text: "Fusion FFmpeg Studio"
                    }

                    Button {
                        text: "X"
                        onClicked: {
                            root.close()
                        }
                    }
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
                areaText: controller.readResourceFile(":/documents/txt/LICENSE")
                fontPointSize: 16
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
