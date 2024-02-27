import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Rectangle {
    color: "#2e2e2e"

    RowLayout {
        anchors.fill: parent

        Text {
            Layout.minimumWidth: 50
            Layout.minimumHeight: 18
            horizontalAlignment: Text.AlignRight
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
            Layout.minimumWidth: 50
            Layout.minimumHeight: 18
            horizontalAlignment: Text.AlignLeft
            text: {
                var m = Math.floor(mediaPlayer.duration / 60000)
                var ms = (mediaPlayer.duration / 1000 - m * 60).toFixed(1)
                return `${m}:${ms.padStart(4, 0)}`
            }
        }
    }
}
