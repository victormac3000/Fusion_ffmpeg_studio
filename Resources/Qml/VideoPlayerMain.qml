import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Rectangle {
    width: 800
    height: 600

    function setSource(source) {
        mediaPlayer.source = "file://" + source
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
            color: "green"


            VideoOutput {
                id: videoOutput
                anchors.fill: parent
                fillMode: VideoOutput.PreserveAspectFit
            }
        }

        VideoPlayerSeeker {
            Layout.fillWidth: true
            Layout.maximumHeight: 40
        }

        VideoPlayerControls {
            Layout.fillWidth: true
            Layout.maximumHeight: 60
        }

        VideoPlayerActions {
            Layout.fillWidth: true
            Layout.maximumHeight: 120
        }

    }
}
