import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Rectangle {
    color: "#2e2e2e"

    property string disableMessage: "MEDIA\n NOT\n FOUND"
    property bool disabled: true

    function setDisableMessage(msg) {
        disabled = true
        disableMessage = msg.toUpperCase()
    }

    function setSource(source) {
        disabled = false
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

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: disabled

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

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "black"

                Text {
                    anchors.fill: parent
                    text: disableMessage
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    fontSizeMode: Text.Fit
                    minimumPointSize: 10
                    font.pointSize: 500
                    color: "#FFFFFF"
                }
            }
        }



        VideoPlayerSeeker {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 40
            Layout.minimumHeight: 40
            enabled: !disabled
        }

        VideoPlayerControls {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 60
            Layout.maximumHeight: 60
            Layout.minimumWidth: 750
            enabled: !disabled
        }

    }
}
