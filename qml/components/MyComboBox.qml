import QtQuick 2.15
import QtQuick.Controls 2.15

ComboBox {
    id: root

    property bool defaultFirst: true

    // Crucial: Tells the ComboBox which model property represents the display text
    textRole: "text"

    model: ListModel {
        id: listModel
    }

    editText: "Henlo"

    background: Rectangle {
        color: "blue"
    }

    currentIndex: {
        /*
        if (model.count > 0) {
            return defaultFirst ? 0 : model.count-1
        }
        return -1
        */
    }

    // Customize the drop-down items to read your appended color properties
    delegate: ItemDelegate {
        height: 25
        width: root.width
        highlighted: root.highlightedIndex === index

        contentItem: Text {
            text: model.text
            // Read textColor from your appended JS object
            color: model.textColor
            font: root.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }

        background: Rectangle {
            width: parent.width
            height: parent.height
            // Read backgroundColor from your appended JS object
            color: model.backgroundColor

            // Optional: visual feedback when hovering/navigating the dropdown list
            border.color: parent.highlighted ? "darkgray" : "transparent"
        }
    }
}