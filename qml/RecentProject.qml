import QtQuick 2.1
import QtQuick.Controls 2.15
import QtQuick.Layouts

Rectangle {
    property string name: "Sample project"
    property string path: "/Users/username/randomfolder/sproj.pro"
    property string rectColor: "#00C0FF"
    property string rectColorHover: "#8AE2FF"


    id: rectangle
    Layout.minimumHeight: 100
    Layout.maximumHeight: 100
    Layout.fillWidth: true
    color: rectColor

    MouseArea {
        hoverEnabled: true
        anchors.fill: parent
        onEntered: {
            parent.color = rectColorHover
        }
        onExited: {
            parent.color = rectColor
        }
        onClicked: {
            parent.parent.recentProjectClicked(parent)
        }
    }

    RowLayout {
        anchors.fill: parent

        Text {
            id: capitalsLetters
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 10
            Layout.maximumWidth: parent.width * 0.2
            text: getText()
            font.bold: true
            font.family: "Arial"
            font.pointSize: 600
            minimumPointSize: 10
            fontSizeMode: Text.Fit
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            Rectangle {
                anchors.fill: parent
                border.color: "black"
                border.width: 5
                color: "#eb7762"
                z: -1
            }

            function getText() {
                let parts = rectangle.name.toUpperCase().trim().split(" ")
                if (parts.length < 2) {
                    let first = -1;
                    let second = -1;
                    for (let i=0; i<rectangle.name.length; i++) {
                        let character = rectangle.name.charAt(i);
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
                        return rectangle.name.charAt(first) + rectangle.name.charAt(second)
                    } else {
                        return rectangle.name.toUpperCase().slice(0,2)
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
