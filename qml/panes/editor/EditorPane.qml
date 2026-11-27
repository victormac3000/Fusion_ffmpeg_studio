import QtQuick
import QtQuick.Layouts

import FusionFFmpegStudio

Rectangle {
    id: root
    color: "#2e2e2e"

    Component.onCompleted: {
        parent.windowSetLimits(0, 0, 800, 600)
    }

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        spacing: 0

        EditorPaneHeader {
            id: header
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            Layout.maximumHeight: 60
        }

        StackLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            currentIndex: header.currentIndex

            EditorPaneEdit {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            EditorPaneRender {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }
    }
}
