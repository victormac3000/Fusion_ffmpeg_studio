import QtQuick
import QtQuick.Controls

Item {
    id: root

    property string spinnerText: qsTr("Loading...")
    property color spinnerColor: "#555555"
    property int spinnerSize: 40
    property int lineWidth: 4

    Column {
        anchors.centerIn: parent
        spacing: 10

        Text {
            id: textLabel

            width: parent.width

            text: root.spinnerText
            color: root.spinnerColor
            font.pixelSize: 16

            horizontalAlignment: Text.AlignHCenter
        }

        Item {
            width: root.spinnerSize
            height: root.spinnerSize

            anchors.horizontalCenter: parent.horizontalCenter

            Canvas {
                id: canvas

                anchors.fill: parent

                onRotationChanged: requestPaint()

                onPaint: {
                    var ctx = getContext("2d")

                    ctx.clearRect(0, 0, width, height)

                    var centerX = width / 2
                    var centerY = height / 2
                    var radius = Math.min(width, height) / 2 - root.lineWidth / 2

                    ctx.beginPath()

                    ctx.arc(
                        centerX,
                        centerY,
                        radius,
                        0,
                        Math.PI * 1.5
                    )

                    ctx.lineWidth = root.lineWidth
                    ctx.lineCap = "round"
                    ctx.strokeStyle = root.spinnerColor
                    ctx.stroke()
                }

                RotationAnimator on rotation {
                    from: 0
                    to: 360
                    duration: 900
                    loops: Animation.Infinite
                    running: true
                }
            }
        }
    }
}