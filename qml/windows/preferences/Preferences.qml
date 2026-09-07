import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import FusionFFmpegStudio

Window {
    id: root
    color: "lightblue"

    required property var parentWindow

    width: parentWindow.width * 0.9
    height: parentWindow.height * 0.9

    maximumHeight: parentWindow.height * 0.9
    maximumWidth: parentWindow.width * 0.9

    minimumHeight: parentWindow.minimumHeight * 0.9
    minimumWidth: parentWindow.minimumWidth * 0.9

    PreferencesController {
        id: controller
        appController: mainController
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredHeight: 36
            Layout.maximumHeight: 36
            Layout.margins: 10

            TabBar {
                id: aboutTabBar
                Layout.fillWidth: true


                TabButton {
                    Layout.fillWidth: true
                    text: qsTr("General")
                }

                TabButton {
                    Layout.fillWidth: true
                    text: qsTr("Rendering")
                }
            }

            Button {
                Layout.preferredWidth: 40
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter
                text: "X"
                onPressed: {
                    root.close()
                }
            }
        }

        StackLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 10
            currentIndex: aboutTabBar.currentIndex

            PreferencesGeneral {
                id: preferencesGeneralArea
                Layout.fillHeight: true
                paneController: controller
                parentWindow: root.parentWindow
            }

            PreferencesRendering {
                id: preferencesRenderingArea
                Layout.fillHeight: true
                paneController: controller
            }
        }
    }
}
