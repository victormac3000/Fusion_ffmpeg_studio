import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Rectangle {
    color: "#2e2e2e"

    RowLayout {
        anchors.fill: parent

        VideoPlayerCutMode {
            Layout.fillWidth: true
            Layout.minimumHeight: 50
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

        VideoPlayerViewMode {
            Layout.fillWidth: true
            Layout.minimumHeight: 50
        }
    }
}
