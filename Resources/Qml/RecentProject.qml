import QtQuick 2.1
import QtQuick.Controls 2.15
import QtQuick.Layouts

Rectangle {
    property string name: "Project name"
    property string path: "/Users/Victor/x/y/z/sjskdjhd/s.pro"
    property string rectColor: "grey"


    id: rectangle
    color: rectColor

    Layout.minimumHeight: 100
    Layout.maximumHeight: 100
    Layout.fillWidth: true

    MouseArea {
        hoverEnabled: true
        anchors.fill: parent
        onEntered: {
            parent.color = "green"
        }
        onExited: {
            parent.color = "grey"
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
            text: "AE"
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
