import QtQuick 2.1
import QtQuick.Controls 2.15
import QtQuick.Layouts

import FusionFFmpegStudio

Rectangle {
    property string uuid: ""
    property string name: ""
    property string path: ""
    property string rectColor: "#00C0FF"
    property string rectColorHover: "#8AE2FF"
    property var clickHandler

    id: root
    color: rectColor

    MouseArea {
        hoverEnabled: true
        anchors.fill: root
        onEntered: {
            root.color = rectColorHover
        }
        onExited: {
            root.color = rectColor
        }
        onClicked: {
            clickHandler(uuid, name, path)
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
    }


}
