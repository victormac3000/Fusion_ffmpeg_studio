import QtQuick 2.15
import QtQuick.Controls 2.15

Slider {
    from: 0
    to: 100
    value: 40

    property string handleColor: "dodgerblue"

    handle: Rectangle {
        width: 20
        height: 20
        radius: 10
        color: handleColor
        border.color: "black"
        anchors.verticalCenter: parent.verticalCenter
        x: {
            let posX = ((parent.width*parent.value) / parent.to) - width/2
            return posX
        }
    }
}
