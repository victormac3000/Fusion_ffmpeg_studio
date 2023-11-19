import QtQuick 6.5
import QtQuick.Layouts
import QtQuick.Controls 2.15

Rectangle {
    property bool selected: false
    property string idString
    property string imagePath
    property string recorded

    height: 200
    width: 200
    border.color: { selected ? "red" : "green"}
    border.width: 10
    radius: 20
    color: "blue"


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
        anchors.fill: parent
        onClicked: {
            videosGridLayout.videoItemClicked(parent)
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
            source: { imagePath ? imagePath : "Icons/VideoPlayer/no_video.png" }
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
                text: { recorded ? recorded : "DD/MM/YYYY" }
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
                text: { idString ? idString : "VIDEO XX" }
                horizontalAlignment: Text.AlignRight
                font.family: "Helvetica"
                font.pointSize: 15
                fontSizeMode: Text.Fit
                rightPadding: 10
            }
        }
    }
}
