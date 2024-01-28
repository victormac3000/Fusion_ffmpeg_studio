import QtQuick 6.5
import QtQuick.Layouts
import QtQuick.Controls 2.15

Rectangle {
    property bool selected: false
    property string idString: "000"
    property string imagePath: "Icons/VideoPlayer/no_video.png"
    property string recorded: "DD/MM/YYYY"


    Layout.minimumHeight: 125
    Layout.minimumWidth: 125
    Layout.fillHeight: true
    Layout.fillWidth: true

    border.color: { selected ? "red" : "green"}
    border.width: 10
    radius: 20
    color: "#0000FF"

    function getModel() {
        return videoData;
    }

    function setSelected(selected) {
       this.selected = selected
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            parent.parent.videoItemClicked(parent)
        }
    }

    ColumnLayout {
        height: parent.height
        width: parent.width
        spacing: 0

        Image {
            Layout.margins: 15
            Layout.fillWidth: true
            Layout.fillHeight: true
            objectName: "image"
            source: imagePath
            fillMode: Image.PreserveAspectFit
        }

        RowLayout {
            Layout.margins: 15
            Layout.fillWidth: true
            Layout.fillHeight: true

            Text {
                id: videoDate
                color: "#ffffff"
                Layout.fillWidth: true
                objectName: "date"
                text: recorded
                font.family: "Helvetica"
                font.pointSize: 15
                fontSizeMode: Text.Fit
                horizontalAlignment: Text.AlignLeft
                leftPadding: 0
            }

            Text {
                id: videoName
                color: "#ffffff"
                Layout.fillWidth: true
                objectName: "name"
                text: idString
                horizontalAlignment: Text.AlignRight
                font.family: "Helvetica"
                font.pointSize: 15
                fontSizeMode: Text.Fit
                rightPadding: 0
            }
        }
    }
}
