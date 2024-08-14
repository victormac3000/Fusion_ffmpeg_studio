import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: root
    width: 600
    height: 400
    color: "lightblue"

    Constants {
        id: constants
        visible: false
    }

    ColumnLayout {
        anchors.fill: parent

        TabBar {
            id: aboutTabBar
            Layout.fillWidth: true
            Layout.preferredHeight: 36
            Layout.margins: 5

            TabButton {
                Layout.fillWidth: true
                text: qsTr("General")
                //font.pointSize: font.pointSize * constants.fontSizeScale
            }

            TabButton {
                Layout.fillWidth: true
                text: qsTr("Rendering")
                //font.pointSize: font.pointSize * constants.fontSizeScale
            }
        }

        StackLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 5
            currentIndex: aboutTabBar.currentIndex

            PreferencesGeneral {
                id: preferencesGeneralArea
                objectName: "preferencesGeneralArea"
                Layout.fillHeight: true
            }

            PreferencesRendering {
                id: preferencesRenderingArea
                objectName: "preferencesRenderingArea"
                Layout.fillHeight: true
            }
        }
    }
}
