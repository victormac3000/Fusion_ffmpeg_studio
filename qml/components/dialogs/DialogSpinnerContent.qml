import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import FusionFFmpegStudio

Item {
    id: root

    property string message: ""

    Spinner {
        id: spinner
        anchors.fill: parent
        spinnerText: root.message
        spinnerSize: 80
        lineWidth: 5
    }
}