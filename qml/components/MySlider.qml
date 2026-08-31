import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Slider {
    from: 0
    to: 100
    value: 50

    property string handleColor: "dodgerblue"
    property string handleBorderColor: "black"

    Layout.margins: 10

    onValueChanged: {
       console.log(value)
    }

    handle: Rectangle {
        width: 20
        height: 20
        radius: 10
        color: handleColor
        border.color: handleBorderColor
        anchors.verticalCenter: parent.verticalCenter
        x: {
            let posX = ((parent.value*parent.width) / parent.to) - width/2
            return posX
        }
    }
}
