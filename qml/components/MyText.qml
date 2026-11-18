import QtQuick 2.15

import FusionFFmpegStudio

Text {
    id: root

    property color defaultColor: "#F6FBF9"
    property bool autoScale: false

    color: defaultColor
    textFormat: Text.PlainText

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
