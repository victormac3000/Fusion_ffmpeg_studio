import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property string spinnerText
    property color spinnerTextColor: "black"
    property color spinnerColor: "#555555"
    property int spinnerSize: 40
    property int lineWidth: 4

    // Optional text drawn inside the ring. Empty string -> nothing is drawn.
    property string spinnerInnerText: ""
    property color spinnerInnerTextColor: "red"

    // Size of the inner text box, expressed as a percentage (1..100) of the
    // inscribed-safe square inside the ring. 100 = current default (full safe
    // area). Values below 1 (e.g. 0, negative, or "not set") and values above
    // 100 fall back to 100.
    property int spinnerInnerSize: 100

    readonly property real _innerSizeRatio: {
        var v = root.spinnerInnerSize
        if (v < 1 || v > 100)
            v = 100
        // 0.7 * diameter is a square fully inscribed in the circle
        return 0.7 * (v / 100.0)
    }

    property bool toolTip: false

    // Sane size when the Spinner is NOT stretched by a layout.
    implicitWidth: rowLayout.implicitWidth
    implicitHeight: rowLayout.implicitHeight

    ColumnLayout {
        id: rowLayout
        anchors.fill: parent
        spacing: 10

        // Label sits ABOVE the spinner. Hidden (and removed from the layout)
        // when there is nothing to show.
        MyText {
            id: textLabel

            visible: root.spinnerText.length > 0

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumWidth: 0        // allow shrinking, otherwise the huge
            Layout.minimumHeight: 0       // implicit size of Text overflows
            Layout.preferredWidth: root.spinnerSize * 2
            Layout.preferredHeight: root.spinnerSize

            text: root.spinnerText
            color: root.spinnerTextColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            autoScale: true
            minimumPointSize: 1           // MyText hard-codes 10 -> overflow
        }

        Item {
            id: spinnerItem

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumWidth: 0
            Layout.minimumHeight: 0
            Layout.preferredWidth: root.spinnerSize
            Layout.preferredHeight: root.spinnerSize

            readonly property real diameter: Math.min(width, height)

            // Square item: ring and inner text always share one center and the
            // circle is always `diameter`, whatever the layout does.
            Item {
                id: circleItem

                anchors.centerIn: parent
                width: spinnerItem.diameter
                height: spinnerItem.diameter
                visible: width > 1 && height > 1

                Canvas {
                    id: canvas
                    anchors.fill: parent

                    onRotationChanged: requestPaint()

                    onPaint: {
                        var ctx = getContext("2d")
                        var d = Math.min(width, height)

                        ctx.clearRect(0, 0, width, height)

                        var lw = Math.max(1, Math.min(root.lineWidth, d / 5))
                        var radius = d / 2 - lw / 2
                        if (radius <= 0)
                            return

                        ctx.beginPath()
                        ctx.arc(width / 2, height / 2, radius, 0, Math.PI * 1.5)
                        ctx.lineWidth = lw
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

                // Optional inner text (e.g. "99%"). Invisible when empty.
                MyText {
                    id: percentText

                    visible: root.spinnerInnerText.length > 0

                    // The box size is driven by spinnerInnerSize (1..100 %).
                    anchors.centerIn: parent
                    width:  parent.width  * root._innerSizeRatio
                    height: parent.height * root._innerSizeRatio

                    text: root.spinnerInnerText
                    color: root.spinnerInnerTextColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    autoScale: true
                    minimumPointSize: 1       // let Text.Fit shrink it all the way
                    wrapMode: Text.NoWrap

                    MouseArea {
                        id: spinnerMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                    }

                    ToolTip.text: root.spinnerInnerText
                    ToolTip.visible: root.toolTip && spinnerMouseArea.containsMouse
                }
            }
        }
    }
}