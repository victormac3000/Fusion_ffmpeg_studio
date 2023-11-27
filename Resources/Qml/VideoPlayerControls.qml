import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Rectangle {
    height: 400
    width: 600

    RowLayout {
        anchors.centerIn: parent
        width: parent.width
        height: 40

        VideoPlayerCutMode {
            height: parent.height
            Layout.fillWidth: true
            Layout.maximumWidth: 280
        }

        Item {
            Layout.fillWidth: true
        }

        RowLayout {
            id: controlButtons
            height: parent.height
            Layout.fillWidth: true

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

        Item {
            Layout.fillWidth: true
        }

        VideoPlayerViewMode {
            height: parent.height
            Layout.fillWidth: true
            Layout.maximumWidth: 280
        }
    }
}
