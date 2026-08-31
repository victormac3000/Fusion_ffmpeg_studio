import QtQuick 2.15
import QtQuick.Controls 2.15

RangeSlider {
    from: 0
    to: 100
    first.value: 40
    second.value: 80

    property string firstHandleColor: "dodgerblue"
    property string secondHandleColor: "dodgerblue"

    first.handle: Rectangle {
        width: 20
        height: 20
        radius: 10
        color: parent.firstHandleColor
        border.color: "black"
        anchors.verticalCenter: parent.verticalCenter
        x: {
            let xPos = ((parent.width*parent.first.value) / parent.to) - width/2
            if ((xPos + width/2) >= (parent.second.handle.x - parent.second.handle.width/2)) {
                return parent.second.handle.x - parent.second.handle.width/2 - width/2
            }
            return xPos
        }
    }

    second.handle: Rectangle {
        width: 20
        height: 20
        radius: 10
        color: parent.secondHandleColor
        border.color: "black"
        anchors.verticalCenter: parent.verticalCenter
        x: {
            let xPos = ((parent.width*parent.second.value) / parent.to) - width/2
            if ((xPos - width/2) <= (parent.first.handle.x + parent.first.handle.width/2)) {
                return parent.first.handle.x + parent.first.handle.width/2 + width/2
            }
            return xPos
        }
    }
}
