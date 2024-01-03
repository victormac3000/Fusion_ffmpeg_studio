import QtQuick 6.5
import QtQuick.Layouts
import QtQuick.Controls 2.15
import es.victor.components

Rectangle {
    property bool selected: false

    width: 125
    height: 125

    border.color: { videoData.selected ? "red" : "green"}
    border.width: 10
    radius: 20
    color: "#0000FF"

    VideoItemModel {
        id: videoData

        function setSelected(selected) {
            this.selected = selected;
        }
    }

    function getModel() {
        return videoData;
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            videosGridView.videoItemClicked(this)
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
            source: videoData.imagePath
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
                text: videoData.recorded
                font.family: "Helvetica"
                font.pointSize: 9
                fontSizeMode: Text.Fit
                horizontalAlignment: Text.AlignLeft
                leftPadding: 0
            }

            Text {
                id: videoName
                color: "#ffffff"
                Layout.fillWidth: true
                objectName: "name"
                text: videoData.idString
                horizontalAlignment: Text.AlignRight
                font.family: "Helvetica"
                font.pointSize: 9
                fontSizeMode: Text.Fit
                rightPadding: 0
            }
        }
    }
}
