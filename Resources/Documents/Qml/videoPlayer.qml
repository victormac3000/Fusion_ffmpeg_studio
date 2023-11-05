import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts
import QtMultimedia
import QtQuick.Window

Rectangle {
    color: "yellow"

    ColumnLayout {
        anchors.fill: parent

        MediaPlayer {
            id: mediaPlayer
            objectName: "videoPlayer"
            audioOutput: AudioOutput {}
            videoOutput: videoOutput
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true

            VideoOutput {
                id: videoOutput
                anchors.fill: parent
                fillMode: VideoOutput.PreserveAspectFit
            }
        }

        Rectangle {
            color: "lightBlue"
            Layout.fillWidth: true
            Layout.minimumHeight: 22
            Layout.maximumHeight: 50

            RowLayout {
                width: parent.width

                Text {
                    text: {
                        var m = Math.floor(mediaPlayer.position / 60000)
                        var ms = (mediaPlayer.position / 1000 - m * 60).toFixed(1)
                        return `${m}:${ms.padStart(4, 0)}`
                    }
                }

                Slider {
                    Layout.fillWidth: true
                    id: slider
                    value: mediaPlayer.position / mediaPlayer.duration
                    onMoved: {
                        mediaPlayer.setPosition(value * mediaPlayer.duration)
                    }
                }

                Text {
                    text: {
                        var m = Math.floor(mediaPlayer.duration / 60000)
                        var ms = (mediaPlayer.duration / 1000 - m * 60).toFixed(1)
                        return `${m}:${ms.padStart(4, 0)}`
                    }
                }
            }
        }

        Rectangle {
            color: "red"
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 35

            RowLayout {
                width: parent.width

                Button {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    id: rewindButton
                    icon.source: "qrc:/Images/MediaPlayer/Left.png"
                    onClicked: {

                    }
                }
                Button {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    id: playButton
                    icon.source: "qrc:/Images/MediaPlayer/Play.png"
                    onClicked: {
                        mediaPlayer.play()
                    }
                }
                Button {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    id: pauseButton
                    icon.source: "qrc:/Images/MediaPlayer/Pause.png"
                    onClicked: {
                        mediaPlayer.pause()
                    }
                }
                Button {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    id: forwardButton
                    icon.source: "qrc:/Images/MediaPlayer/Right.png"
                    onClicked: {

                    }
                }
            }
        }
    }
}
