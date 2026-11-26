import QtQuick 2.1
import QtQuick.Controls 2.15
import QtQuick.Layouts

import FusionFFmpegStudio

Rectangle {
    property string uuid: ""
    property string name: ""
    property string path: ""
    property color rectColor: "#00C0FF"
    property color rectColorHover: "#8AE2FF"
    property color closeButtonHoverColor: "#ff917e"
    property color rectColorDisabled: "#b8a5a5"
    property bool valid: true
    property var clickHandler
    property var removeClickHandler

    id: root
    color: valid ? rectColor : rectColorDisabled

    MouseArea {
        id: cardMouseArea

        anchors.fill: parent
        hoverEnabled: true

        cursorShape: (root.valid ||
                      (containsMouse && isInsideCloseButton(mouseX, mouseY)))
                     ? Qt.PointingHandCursor
                     : Qt.ArrowCursor

        function isInsideCloseButton(x, y) {
            const pos = closeBtnRectangle.mapToItem(cardMouseArea, 0, 0)

            return x >= pos.x &&
                   x <= pos.x + closeBtnRectangle.width &&
                   y >= pos.y &&
                   y <= pos.y + closeBtnRectangle.height
        }

        onPositionChanged: function(mouse) {
            const inside = isInsideCloseButton(mouse.x, mouse.y)
            closeBtnRectangle.color = inside ? root.closeButtonHoverColor : "transparent"
        }

        onEntered: {
            if (!root.valid) return
            root.color = root.rectColorHover
        }

        onExited: {
            if (!root.valid) return
            root.color = root.rectColor
        }

        onClicked: function(mouse) {
            const insideCloseRect = isInsideCloseButton(mouse.x, mouse.y)

            if (insideCloseRect) {
                root.removeClickHandler(root.uuid)
                return
            }

            if (root.valid) {
                root.clickHandler(root.uuid, root.name, root.path)
            }
        }
    }

    RowLayout {
        id: rowLayout
        anchors.fill: root

        Text {
            id: capitalsLetters
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 10
            Layout.maximumWidth: rowLayout.width * 0.2
            text: getText()
            font.bold: true
            font.family: "Arial"
            font.pointSize: 600
            minimumPointSize: 10
            fontSizeMode: Text.Fit
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            Rectangle {
                anchors.fill: capitalsLetters
                border.color: "black"
                border.width: 5
                color: "#eb7762"
                z: -1
            }

            function getText() {
                let parts = root.name.toUpperCase().trim().split(" ")
                if (parts.length < 2) {
                    let first = -1;
                    let second = -1;
                    for (let i=0; i<root.name.length; i++) {
                        let character = root.name.charAt(i);
                        if (character == character.toUpperCase()) {
                            if (first >= 0) {
                                second = i;
                                break;
                            }
                            first = i;
                            continue;
                        }
                    }

                    if (first >= 0 && second >= 0) {
                        return root.name.charAt(first) + root.name.charAt(second)
                    } else {
                        return root.name.toUpperCase().slice(0,2)
                    }
                }
                return parts[0].charAt(0) + parts[1].charAt(0)
            }
        }


        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 10

            Text {
                id: title
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: name
                verticalAlignment: Text.AlignBottom
                font.bold: true
                font.pointSize: 600
                minimumPointSize: 10
                fontSizeMode: Text.Fit
                font.family: "Arial"
            }

            Text {
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: path
                verticalAlignment: Text.AlignTop
                font.pixelSize: 300
                minimumPointSize: 5
                fontSizeMode: Text.Fit
                font.family: "Arial"
            }
        }

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredWidth: 60
            Layout.margins: 10
            Layout.rightMargin: 30


            Rectangle {
                id: closeBtnRectangle

                Layout.preferredHeight: rowLayout.height * 0.4
                Layout.fillWidth: true

                radius: 180
                color: "transparent"


                MyText {
                    anchors.fill: parent

                    autoScale: true
                    text: "×"
                    color: "black"
                    font.family: "Arial"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
}
