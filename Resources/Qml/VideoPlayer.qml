import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Rectangle {
    property string disableMessage: "MEDIA\n NOT\n FOUND"
    property bool disabled: true

    color: "#2e2e2e"

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
        id: videoPlayerLayout

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

        ColumnLayout {
            property bool cutMode: false

            function setCutMode(cutModeSet) {
                cutMode = cutModeSet
            }

            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: mediaPlayer.duration > 0

            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: 40
                Layout.maximumHeight: 40
                Layout.bottomMargin: 20

                Text {
                    Layout.minimumWidth: 50
                    Layout.minimumHeight: 18
                    horizontalAlignment: Text.AlignRight
                    color: "#e8ffeaea"
                    font.family: "Courier New"
                    text: {
                        var miliseconds = videoPlayerSeekerBar.barValue
                        var seconds = Math.floor(miliseconds/1000)
                        miliseconds %= 1000
                        var minutes = Math.floor(seconds/60)
                        seconds %= 60
                        var hours = Math.floor(minutes/60)
                        minutes %= 60
                        return String(hours).padStart(2, "0") + ":" +
                               String(minutes).padStart(2, "0") + ":" +
                               String(seconds).padStart(2, "0") + "." +
                               String(miliseconds).charAt(0)
                    }
                }

                VideoPlayerSeekerBar {
                    id: videoPlayerSeekerBar
                    objectName: "videoPlayerSeekerBar"
                    Layout.fillHeight: true
                    Layout.minimumHeight: 18
                    Layout.fillWidth: true
                    rangeFrom: 0
                    rangeTo: mediaPlayer.duration
                    barValue: mediaPlayer.position
                    cutMode: parent.parent.cutMode
                }

                Text {
                    Layout.minimumWidth: 50
                    Layout.minimumHeight: 18
                    horizontalAlignment: Text.AlignLeft
                    color: "#e8ffeaea"
                    font.family: "Courier New"
                    text: {
                        var miliseconds = videoPlayerSeekerBar.rangeTo
                        var seconds = Math.floor(miliseconds/1000)
                        miliseconds %= 1000
                        var minutes = Math.floor(seconds/60)
                        seconds %= 60
                        var hours = Math.floor(minutes/60)
                        minutes %= 60
                        return String(hours).padStart(2, "0") + ":" +
                               String(minutes).padStart(2, "0") + ":" +
                               String(seconds).padStart(2, "0") + "." +
                               String(miliseconds).charAt(0)
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: 60
                Layout.maximumHeight: 60

                RowLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: 60

                    Text {
                        text: "Full video"
                        color: "#fefefe"
                    }

                    Switch {
                        onPositionChanged: {
                            parent.parent.parent.setCutMode(position)
                        }
                    }

                    Text {
                        text: "Video segment"
                        color: "#fefefe"
                    }

                    Item {
                        Layout.fillWidth: true
                    }
                }

                RowLayout {
                    id: controlButtons
                    Layout.fillWidth: true
                    Layout.minimumHeight: 50

                    RoundButton {
                        id: frameBackButton
                        radius: 50.0
                        text: "🔙";
                        onClicked: {
                            mediaPlayer.pause()
                            mediaPlayer.setPosition(mediaPlayer.position - 1/mediaPlayer.duration);
                        }
                    }

                    RoundButton {
                        id: playButton
                        radius: 50.0
                        text: "\u25B6";
                        onClicked: {
                            mediaPlayer.play()
                        }
                    }

                    RoundButton {
                        id: pauseButton
                        radius: 50.0
                        text: "\u2016";
                        onClicked: {
                            mediaPlayer.pause()
                        }
                    }

                    RoundButton {
                        id: frameFrontButton
                        radius: 50.0
                        text: "🔜";
                        onClicked: {
                            mediaPlayer.pause()
                            mediaPlayer.setPosition(mediaPlayer.position + 1/mediaPlayer.duration);
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: 50

                    Item {
                        Layout.fillWidth: true
                    }

                    Text {
                        text: "Equirectangular"
                        color: "#fefefe"
                    }

                    Switch {

                    }

                    Text {
                        text: "Interactive 360"
                        color: "#fefefe"
                    }
                }
            }
        }
    }
}
