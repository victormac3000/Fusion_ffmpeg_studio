import QtQuick
import QtQuick.Controls

import FusionFFmpegStudio

Switch {
    id: root

    // ── Colors ───────────────────────────────────────────────
    property color trackColorOff:   "#555555"
    property color trackColorOn:    "#00A1E1"
    property color knobColorOff:    "#dddddd"
    property color knobColorOn:     "#ffffff"
    property color labelColorOff:   "#F6FBF9"
    property color labelColorOn:    "#00A1E1"

    // ── Track geometry ───────────────────────────────────────
    property int   trackWidth:       44      // full pill width
    property int   trackHeight:      22      // full pill height
    property real  trackRadius:      trackHeight / 2   // override for square-ish tracks

    // ── Track border ─────────────────────────────────────────
    property int   trackBorderWidth: 1
    property color trackBorderColorOn:  trackColorOn
    property color trackBorderColorOff: trackColorOff

    // ── Knob ─────────────────────────────────────────────────
    property real  knobMargin:       2       // gap between knob and inner track edge

    indicator: Rectangle {
        implicitWidth:  root.trackWidth
        implicitHeight: root.trackHeight
        width:          root.trackWidth          // pin actual size to the parameter
        height:         root.trackHeight
        x: root.leftPadding
        y: parent.height / 2 - height / 2
        radius: root.trackRadius

        color:        root.checked ? root.trackColorOn        : root.trackColorOff
        border.width: root.trackBorderWidth
        border.color: root.checked ? root.trackBorderColorOn  : root.trackBorderColorOff

        // Knob = inner height minus margins on both sides
        readonly property real knobSize:
            Math.max(0, height - 2 * (root.trackBorderWidth + root.knobMargin))

        Rectangle {
            x: root.checked
               ? parent.width  - root.trackBorderWidth - root.knobMargin - parent.knobSize
               :                 root.trackBorderWidth + root.knobMargin
            y: root.trackBorderWidth + root.knobMargin

            width:  parent.knobSize
            height: parent.knobSize
            radius: height / 2
            color: root.checked ? root.knobColorOn : root.knobColorOff

            Behavior on x {
                NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
            }
        }
    }

    contentItem: Text {
        text:  root.text
        font:  root.font
        color: root.checked ? root.labelColorOn : root.labelColorOff
        verticalAlignment: Text.AlignVCenter
        leftPadding: root.indicator.width + root.spacing
    }
}