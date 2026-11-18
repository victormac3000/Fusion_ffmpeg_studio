import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import FusionFFmpegStudio

Rectangle {
    id: root
    color: "#000000"
    border.width: selected ? 3 : 0
    border.color: "#00A1E1"

    required property var video
    required property var canSelect
    required property var canDeselect
    property bool selected: false

    signal onDataChanged(data: var)

    onSelectedChanged: {
        if (selected) {
            border.width = 3
            image.Layout.rightMargin = 3
            image.Layout.leftMargin = 3
        } else {
            border.width = 0
            image.Layout.rightMargin = 0
            image.Layout.leftMargin = 0
        }
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            if (root.selected && canDeselect(root)) {
                root.selected = false
                return
            }
            if (!root.selected && canSelect(root)) {
                root.selected = true
                return
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        RowLayout {
            spacing: 5
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height*0.1

            Layout.margins: 10 + root.border.width
            Layout.bottomMargin: 5 + root.border.width
            Layout.topMargin: 5 + root.border.width

            MyTextField {
                Layout.fillHeight: true
                Layout.fillWidth: true
                autoScale: true

                bottomPadding: 0

                font.bold: true
                text: qsTr("VIDEO") + " " + root.video.idString
                horizontalAlignment: Text.AlignLeft
                maximumLength: 100
                idleColor:   "#F6FBF9"
                hoverColor:  "#00A1E1"
                editingColor:"#00A1E1"
                minimumLength: 1                

                onTextChanged: {
                    root.video.name = text
                }

                onCommitted: (value) => {
                    root.video.name = value
                    onDataChanged(root.video)
                }
            }

            MyText {
                id: textLabel
                Layout.fillHeight: true
                Layout.fillWidth: false
                Layout.maximumWidth: parent.width*0.4
                autoScale: true
                elide: Text.ElideRight
                Layout.alignment: Qt.AlignVCenter
                wrapMode: Text.NoWrap
                verticalAlignment: Text.AlignVCenter
                text: Qt.formatDateTime(root.video.recordedOn, "dd/MM/yyyy")

            }
        }

        StackLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height*0.8

            Layout.rightMargin: root.border.width
            Layout.leftMargin: root.border.width
            Layout.topMargin: 0
            Layout.bottomMargin: 0


            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: "blue"

                Spinner {
                    anchors.fill: parent
                    anchors.margins: parent.width*0.05
                    spinnerInnerText: "0%"
                    spinnerInnerSize: 75
                }
            }

            Image {
                id: image
                Layout.fillHeight: true
                Layout.preferredWidth:  Layout.preferredHeight   // keep square
                Layout.maximumWidth:  implicitWidth
                Layout.maximumHeight: implicitHeight
                fillMode: Image.Stretch
                source: "qrc:/images/Snow.jpg"
            }
        }


        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height*0.1

            Layout.margins: 10 + root.border.width
            Layout.bottomMargin: 5 + root.border.width
            Layout.topMargin: 5 + root.border.width

            Image {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumWidth: parent.height
                Layout.rightMargin: 10
                Layout.topMargin: 0
                fillMode: Image.PreserveAspectFit
                source: "qrc:/icons/editor/picture-frame-white.png"
            }

            MyText {
                Layout.fillWidth: true
                Layout.fillHeight: true
                autoScale: true
                text: root.video.durationString
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
