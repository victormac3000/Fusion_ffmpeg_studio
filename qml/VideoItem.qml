import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

/*
  <a href="https://www.flaticon.es/iconos-gratis/informacion" title="información iconos">Información iconos creados por Freepik - Flaticon</a>
*/

Rectangle {
    property bool selected: false
    property int id: -1
    property string idString: "000"
    property string b64image
    property string recorded: "DD/MM/YYYY"

    Layout.minimumHeight: 125
    Layout.minimumWidth: 125
    Layout.fillWidth: true

    border.color: { selected ? "red" : "green"}
    border.width: 10
    radius: 20
    color: "#0000FF"

    function setSelected(selected) {
       this.selected = selected
    }

    function setData(id, idString, b64image, rec) {
        if (id >= 0 && idString !== "" && rec !== "") {
            this.id = id
            this.idString = idString
            this.b64image = b64image
            this.recorded = rec
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            parent.parent.videoItemClicked(parent)
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Text {
            Layout.topMargin: 15
            Layout.fillHeight: true
            color: "#ffffff"
            Layout.fillWidth: true
            text: "VIDEO " + idString
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
            source: "data:image/jpeg;base64," + b64image
            fillMode: Image.PreserveAspectFit
        }

        Text {
            Layout.bottomMargin: 30
            Layout.fillHeight: true
            color: "#ffffff"
            text: "DD/MM/YYYY"
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 100
            minimumPointSize: 10
            fontSizeMode: Text.Fit
            Layout.maximumHeight: 0.1*parent.height
        }
    }
}
