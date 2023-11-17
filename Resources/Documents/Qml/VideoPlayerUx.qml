import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts
import QtMultimedia
import QtQuick.Window
import QtQuick.Controls.iOS

Rectangle {
    width: 800
    height: 600
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
                        mediaPlayer.setPosition(first.value * mediaPlayer.duration)
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
            Layout.minimumHeight: 40
            Layout.maximumHeight: 40

            RowLayout {
                width: parent.width
                height: parent.height

                Button {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    id: rewindButton
                    Image {
                        source: "Icons/VideoPlayer/Left.png"
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                        sourceSize.height: rewindButton.background.height - 6
                        height: sourceSize.height
                    }
                }
                Button {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    id: playButton
                    Image {
                        source: "Icons/VideoPlayer/Play.png"
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                        sourceSize.height: playButton.background.height - 6
                        height: sourceSize.height
                    }
                    onClicked: {
                        mediaPlayer.play()
                    }
                }
                Button {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    id: pauseButton
                    Image {
                        source: "Icons/VideoPlayer/Pause.png"
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                        sourceSize.height: pauseButton.background.height - 6
                        height: sourceSize.height
                    }
                    onClicked: {
                        mediaPlayer.pause()
                    }
                }
                Button {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    id: forwardButton
                    Image {
                        source: "Icons/VideoPlayer/Right.png"
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                        sourceSize.height: forwardButton.background.height - 6
                        height: sourceSize.height
                    }
                }
            }
        }
    }
}
