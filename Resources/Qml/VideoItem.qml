import QtQuick 6.5
import QtQuick.Layouts
import QtQuick.Controls 2.15
import es.victor.components

Rectangle {
    property bool selected: false

    height: 200
    width: 200
    border.color: { selected ? "red" : "green"}
    border.width: 10
    radius: 20
    color: "blue"

    VideoItemModel {
        id: videoData
    }

    function getModel() {
        return videoData;
    }

    onWidthChanged: {
        if (width >= 190) {
            videoDate.visible = true
            videoName.horizontalAlignment = Text.AlignLeft
        } else {
            videoDate.visible = false
            videoName.horizontalAlignment = Text.AlignHCenter
        }
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
                font.pointSize: 15
                fontSizeMode: Text.Fit
                horizontalAlignment: Text.AlignLeft
                leftPadding: 10
            }

            Text {
                id: videoName
                color: "#ffffff"
                Layout.fillWidth: true
                objectName: "name"
                text: videoData.idString
                horizontalAlignment: Text.AlignRight
                font.family: "Helvetica"
                font.pointSize: 15
                fontSizeMode: Text.Fit
                rightPadding: 10
            }
        }
    }
}
