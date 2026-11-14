import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import FusionFFmpegStudio

Slider {
    id: slider

    from: 0
    to: 100
    value: 50

    property string handleColor: "#f6fbf9"
    property string handleBorderColor: "#00A0FF"

    handle: Rectangle {
        width: 15
        height: 15
        radius: width / 2
        color: handleColor

        x: parent.leftPadding + parent.visualPosition *
           (parent.availableWidth - width)
        y: parent.topPadding + (parent.availableHeight - height) / 2

        Rectangle {
            width: parent.width/2
            height: parent.height/2
            radius: width / 2
            anchors.centerIn: parent
            color: handleBorderColor
        }
    }
}



