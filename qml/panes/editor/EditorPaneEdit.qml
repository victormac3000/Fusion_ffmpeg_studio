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
            id: videosPane
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        EditorPaneEditPlayer {
            selectedVideo: videosPane.selectedVideo
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
