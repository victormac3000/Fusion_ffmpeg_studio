import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

/*
    <a href="https://iconscout.com/icons/dropdown" class="text-underline font-size-sm" target="_blank">Dropdown</a> by <a href="https://iconscout.com/contributors/jemismali" class="text-underline font-size-sm" target="_blank">Jemis Mali</a>
    <a href="https://iconscout.com/icons/cross" class="text-underline font-size-sm" target="_blank">Cross</a> by <a href="https://iconscout.com/contributors/epangelald" class="text-underline font-size-sm" target="_blank">December</a>
*/

Rectangle {
    property string videoIdString: "000"
    property string operation: "UNKNOWN OPERATION"
    property int currentFrame: 0
    property int totalFrames: 0
    property double fps: 0
    property double quality: 0
    property double size: 0
    property string elapsedTime: "00:00:00"
    property string remainingTime: "00:00:00"
    property int bitrate: 0
    property double speed: 0
    property double percent: {
        if (totalFrames == 0) {
            return 0
        }
        return (currentFrame / totalFrames) * 100
    }

    property bool extended: false

    border.color: "green"
    border.width: 5
    radius: 20
    color: "blue"
    Layout.maximumHeight: { extended ? 360 : 170 }
    Layout.minimumHeight: { extended ? 360 : 170 }
    Layout.minimumWidth: { extended ? 300 : 150 }
    Layout.fillHeight: true
    Layout.fillWidth: true

    RoundButton {
        x: 10
        y: 10
        icon.source: "Icons/Queue/cross.png"
        radius: parent.radius
        icon.color: "#ff0000"

        onClicked: {

        }
    }

    RoundButton {
        x: parent.width - width - 10
        y: 10
        icon.source: "Icons/Queue/dropdown.png"
        radius: parent.radius
        icon.color: "#000000"

        onClicked: {
            extended = !extended;
        }
    }

    ColumnLayout {
        height: parent.height
        width: parent.width
        spacing: 10

        Item {
            height: 20
        }


        Text {
            color: "#ffffff"
            text: "VIDEO " + videoIdString
            Layout.fillWidth: true
            objectName: "date"
            font.family: "Helvetica"
            font.bold: true
            font.pixelSize: 18
            horizontalAlignment: Text.AlignHCenter
            leftPadding: 10
        }

        Text {
            color: "#ffffff"
            text: operation
            Layout.fillWidth: true
            objectName: "date"
            font.family: "Helvetica"
            font.bold: true
            font.pixelSize: 18
            horizontalAlignment: Text.AlignHCenter
            leftPadding: 10
        }

        GridLayout {
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            Layout.margins: 15
            columns: 3
            rows: 2

            Text {
                text: percent + "%"
                font.bold: true
                font.pixelSize: 18
                horizontalAlignment: Text.AlignHCenter
                color: "white"
            }

            NiceProgressBar {
                Layout.fillWidth: true
                Component.onCompleted: {
                    setParam("from", 0)
                    setParam("to", 100)
                    setParam("value", percent)
                    setParam("text", "")
                }
            }

            Text {
                font.bold: true
                font.pixelSize: 18
                color: "white"
                text: "100%"
            }

            Text {
                width: 30
                font.bold: true
                font.pixelSize: 18
                horizontalAlignment: Text.AlignHCenter
                color: "white"
                text: speed + "x"
            }

            NiceProgressBar {
                Layout.fillWidth: true
                Component.onCompleted: {
                    setParam("from", 0)
                    setParam("to", 5)
                    setParam("value", speed > 5.0 ? 5.0 : speed)
                    setParam("text", "vs realtime")
                }
            }

            Text {
                width: 49
                font.bold: true
                font.pixelSize: 18
                horizontalAlignment: Text.AlignHCenter
                color: "white"
                text: "5x"
            }

        }

        GridLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 15
            rows: 9
            columns: 2
            visible: extended

            Text {
                Layout.fillWidth: true
                color: "white"
                text: "FRAMES PROGRESS"
                font.bold: true
                font.pixelSize: 18
                horizontalAlignment: Text.AlignLeft
            }

            Text {
                Layout.fillWidth: true
                color: "white"
                text: currentFrame + " / " + totalFrames + " frames"
                font.bold: true
                font.pixelSize: 18
                horizontalAlignment: Text.AlignRight
            }

            Text {
                Layout.fillWidth: true
                color: "white"
                text: "RENDER FRAMES PER SECOND"
                font.bold: true
                font.pixelSize: 18
                horizontalAlignment: Text.AlignLeft
            }

            Text {
                Layout.fillWidth: true
                color: "white"
                text: fps + " fps"
                font.bold: true
                font.pixelSize: 18
                horizontalAlignment: Text.AlignRight
            }

            Text {
                Layout.fillWidth: true
                color: "white"
                text: "OUTPUT VIDEO SIZE"
                font.bold: true
                font.pixelSize: 18
                horizontalAlignment: Text.AlignLeft
            }

            Text {
                Layout.fillWidth: true
                color: "white"
                text: size + " MB"
                font.bold: true
                font.pixelSize: 18
                horizontalAlignment: Text.AlignRight
            }

            Text {
                Layout.fillWidth: true
                color: "white"
                text: "BITRATE"
                font.bold: true
                font.pixelSize: 18
                horizontalAlignment: Text.AlignLeft
            }

            Text {
                Layout.fillWidth: true
                color: "white"
                text: bitrate + " Mbps"
                font.bold: true
                font.pixelSize: 18
                horizontalAlignment: Text.AlignRight
            }

            Text {
                Layout.fillWidth: true
                color: "white"
                text: "ELAPSED TIME"
                font.bold: true
                font.pixelSize: 18
                horizontalAlignment: Text.AlignLeft
            }

            Text {
                Layout.fillWidth: true
                color: "white"
                text: elapsedTime
                font.bold: true
                font.pixelSize: 18
                horizontalAlignment: Text.AlignRight
            }

            Text {
                Layout.fillWidth: true
                color: "white"
                text: "ESTIMATED REMAINING TIME"
                font.bold: true
                font.pixelSize: 18
                horizontalAlignment: Text.AlignLeft
            }

            Text {
                Layout.fillWidth: true
                color: "white"
                text: remainingTime
                font.bold: true
                font.pixelSize: 18
                horizontalAlignment: Text.AlignRight
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }



}
