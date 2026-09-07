import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Button {
    id: root

    property color disabledColor: "#000eb2"
    property color backgroundDefaultColor: "#4E5BF2"
    property color backgroundHoverColor: "#6571F5"
    property color backgroundPressedColor: Qt.darker(backgroundDefaultColor, 1.2)
    property color contentItemTextColor: "white"

    property int cornerRadius: 3
    property int textPixelSize: 20

    text: "Button"

    hoverEnabled: true

    contentItem: Text {
        id: buttonText

        text: root.text
        color: root.contentItemTextColor

        font.family: "Arial"
        font.weight: Font.Thin
        font.pixelSize: root.textPixelSize

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        elide: Text.ElideRight
        wrapMode: Text.Wrap

        // Don't use anchors.fill here.
        // Let the Button manage the content item's size.
    }

    background: Rectangle {
        radius: root.cornerRadius

        color: {
            if (!root.enabled)
                return root.disabledColor

            if (root.down)
                return root.backgroundPressedColor

            if (root.hovered)
                return root.backgroundHoverColor

            return root.backgroundDefaultColor
        }

        Behavior on color {
            ColorAnimation {
                duration: 100
            }
        }
    }
}
