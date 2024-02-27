import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Rectangle {
    color: "#2e2e2e"

    function setSource(source) {
        mediaPlayer.source = "file://" + source
        mediaPlayer.play()
        mediaPlayer.pause()
    }

    ColumnLayout {
        anchors.fill: parent

        MediaPlayer {
            id: mediaPlayer
            objectName: "mediaPlayer"
            audioOutput: AudioOutput {}
            videoOutput: videoOutput
            source: ""
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "black"

            VideoOutput {
                width: parent.width
                height: parent.height
                id: videoOutput
                fillMode: VideoOutput.PreserveAspectFit
            }
        }

        VideoPlayerSeeker {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 40
            Layout.minimumHeight: 40
        }

        VideoPlayerControls {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 60
            Layout.maximumHeight: 60
            Layout.minimumWidth: 750
        }

    }
}
