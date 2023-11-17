import QtQuick 6.5
import QtQuick.Layouts
import QtQuick.Controls 2.15

Rectangle {
    property int id: -1
    property string idString: "VIDEO N/A"
    property date recorded: Date.fromLocaleDateString("01/01/1970")
    property string imagePath: "Icons/VideoPlayer/no_video.png"
    property bool selected: false

    height: 200
    width: 200
    border.color: { selected ? "red" : "green"}
    border.width: 10
    radius: 20
    color: "blue"

    function init(id, idString, recorded, imagePath) {
        this.id = id
        this.idString = idString
        this.recorded = recorded
        this.imagePath = imagePath
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
                text: recorded.toDateString()
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
                text: idString
                horizontalAlignment: Text.AlignRight
                font.family: "Helvetica"
                font.pointSize: 15
                fontSizeMode: Text.Fit
                rightPadding: 10
            }
        }
    }
}
