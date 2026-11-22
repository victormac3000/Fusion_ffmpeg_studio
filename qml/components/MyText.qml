import QtQuick
import QtQuick.Layouts
import FusionFFmpegStudio

Text {
    id: root

    property color defaultColor: "#F6FBF9"
    property bool autoScale: false

    // Design font size used to compute a stable Layout.preferredWidth in
    // autoScale mode. Match the font size you would use with autoScale off.
    property real autoScaleFloorPointSize: 13

    color: defaultColor
    textFormat: Text.PlainText

    // Measure the string at the *design* font size — never at the fitted size,
    // never at the 600 pt from the state below. This value does not depend on
    // the item's current width, so feeding it into Layout.preferredWidth
    // terminates the chain:
    //
    //   width → Text.Fit → implicitWidth → (default) Layout.preferredWidth
    //         → layout pass → width → …
    //
    // becomes:
    //
    //   width → Text.Fit → implicitWidth     (unused by the layout)
    //   layout ← designMetrics.advanceWidth  (constant)
    TextMetrics {
        id: designMetrics
        font.family:         root.font.family
        font.weight:         root.font.weight
        font.styleName:      root.font.styleName
        font.capitalization: root.font.capitalization
        font.letterSpacing:  root.font.letterSpacing
        font.wordSpacing:    root.font.wordSpacing
        font.pointSize:      root.autoScaleFloorPointSize
        text: root.text.length > 0 ? root.text : "X"
    }

    // Stable preferred width when autoScale is on; original behaviour when off.
    Layout.preferredWidth: root.autoScale ? designMetrics.advanceWidth
                                          : root.implicitWidth

    // Still needed from the previous fix: lets the layout shrink us below
    // the design width when the container is narrower than the string.
    Layout.minimumWidth:  root.autoScale ? 0 : root.implicitWidth
    Layout.minimumHeight: root.autoScale ? 0 : root.implicitHeight

    states: State {
        name: "autoScale"
        when: root.autoScale

        PropertyChanges {
            target: root
            minimumPointSize: 10
            font.pointSize: 600
            fontSizeMode: Text.Fit
        }
    }
}