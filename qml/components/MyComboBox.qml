import QtQuick 2.15
import QtQuick.Controls 2.15

ComboBox {
    id: root
    model: ListModel{}
    textRole: "text"

    property string placeholderText: ""
    property string stateText: ""
    property bool selectDefault: false
    property bool defaultFirst: true

    property color boxBackgroundColor: "white"
    property color boxTextColor: "black"
    property color boxTextColorPlaceholder: "grey"
    property color selectorHighlitedBorderColor: "darkgray"
    property color selectorBorderColor: "transparent"
    property color selectorTextColor: "black"
    property color selectorBackgroundColor: "white"
    property double selectorBorderWidth: 1

    state: "normal"
    states: [
        State {
            name: "normal"
            PropertyChanges {
                target: root
                boxBackgroundColor: "white"
            }
        },
        State {
            name: "warning"
            PropertyChanges {
                target: root
                boxBackgroundColor: "#FFB500"
            }
        },
        State {
            name: "error"
            PropertyChanges {
                target: root
                boxBackgroundColor: "#F5B2AF"

            }
        }
    ]

    onCountChanged: {
        if (selectDefault && count > 0 && currentIndex < 0) {
            currentIndex = defaultFirst ? 0 : count - 1
        }
    }

    onStateChanged: {
        if (state === "normal") {
            stateText = ""
        }
    }


    ToolTip {
        id: tooltip
        visible: (root.state === "error" || root.state === "warning") && root.hovered
        text: root.stateText
        x: (root.width - width) / 2
        y: -height - 5
        enabled: false
    }

    // Customizes the box when closed
    contentItem: Rectangle {
        color: boxBackgroundColor

        Text {
            anchors.fill: parent
            anchors.rightMargin: 10
            anchors.leftMargin: 10

            text: {
                if (root.currentIndex < 0 && root.placeholderText.length > 0) {
                    return root.placeholderText
                }
                return root.currentText
            }

            color: {
                if (root.currentIndex < 0 && root.placeholderText.length > 0) {
                    return root.boxTextColorPlaceholder
                }
                return root.boxTextColor
            }

            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }
    }

    // Customize each element on box list
    delegate: ItemDelegate {
        height: 25
        width: root.width
        highlighted: root.highlightedIndex === index

        contentItem: Text {
            text: model.text || ""
            color: model.textColor || selectorTextColor
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }

        background: Rectangle {
            width: parent.width
            height: parent.height
            color: model.backgroundColor || selectorBackgroundColor
            border.color: parent.highlighted ? selectorHighlitedBorderColor : selectorBorderColor
            border.width: selectorBorderWidth
        }
    }
}