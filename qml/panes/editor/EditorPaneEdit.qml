import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: root
    color: "#141414"

    RowLayout {
        id: editLayout
        anchors.fill: parent
        spacing: 0

        EditorPaneEditVideos {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        EditorPaneEditPlayer {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
