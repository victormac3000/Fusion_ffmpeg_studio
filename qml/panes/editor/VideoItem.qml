import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

/*
  <a href="https://www.flaticon.es/iconos-gratis/informacion" title="información iconos">Información iconos creados por Freepik - Flaticon</a>
*/

Rectangle {
    id: root

    required property var video
    required property var clickHandler
    property bool selected: false

    Layout.minimumHeight: 125
    Layout.minimumWidth: 125
    Layout.fillWidth: true

    border.color: { selected ? "red" : "green"}
    border.width: 5
    radius: 20
    color: "#0000FF"

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            clickHandler(root)
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 0

        Text {
            Layout.fillHeight: true
            color: "#ffffff"
            Layout.fillWidth: true
            text: "VIDEO " + root.video.idString
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 100
            minimumPointSize: 10
            rightPadding: 0
            fontSizeMode: Text.Fit
            Layout.maximumHeight: 0.1*parent.height
        }

        Image {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 5
            source: "data:image/jpeg;base64," + video.frontThumbnailB64
            fillMode: Image.PreserveAspectFit
        }

        Text {
            Layout.fillHeight: true
            color: "#ffffff"
            text: video.recordedOnDate
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 100
            minimumPointSize: 10
            fontSizeMode: Text.Fit
            Layout.maximumHeight: 0.1*parent.height
        }
    }
}
