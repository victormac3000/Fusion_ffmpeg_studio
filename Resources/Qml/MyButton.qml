import QtQuick 2.15
import QtQuick.Controls 2.15

Button {
    id: root
    property color backgroundDefaultColor: "#4E5BF2"
    property color backgroundPressedColor: Qt.darker(backgroundDefaultColor, 1.2)
    property color contentItemTextColor: "white"

    text: "Button"

    contentItem: Item {
        width: parent.width
        height: parent.height
        anchors.centerIn: parent

        Text {
            id: buttonText
            text: root.text
            color: root.contentItemTextColor
            font.family: "Arial"
            font.weight: Font.Thin
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
            anchors.centerIn: parent
            wrapMode: Text.Wrap

            // Adjust font size to fit within the button
            font.pixelSize: Math.min(parent.width, parent.height) * 0.5
            // Ensure font size is not less than a minimum value
            minimumPixelSize: 10
        }
    }

    background: Rectangle {
        width: 83
        height: 37
        color: root.down ? root.backgroundPressedColor : root.backgroundDefaultColor
        radius: 3

        Rectangle {
            anchors.fill: parent
            color: "transparent"
        }
    }
}
