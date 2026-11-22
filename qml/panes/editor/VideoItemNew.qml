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

    MouseArea {
        anchors.fill: parent

        onClicked: {
            if (root.selected && root.canDeselect(root)) {
                root.selected = false
            } else if (!root.selected && root.canSelect(root)) {
                root.selected = true
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        RowLayout {
            id: headerRow
            spacing: 5
            Layout.fillWidth: true
            Layout.preferredHeight: root.height * 0.1

            Layout.margins: 10 + root.border.width
            Layout.bottomMargin: 5 + root.border.width
            Layout.topMargin: 5 + root.border.width

            MyTextField {
                Layout.fillHeight: true
                Layout.fillWidth: true
                autoScale: true

                font.bold: true
                text: qsTr("VIDEO") + " " + root.video.idString
                horizontalAlignment: Text.AlignLeft
                maximumLength: 100
                idleColor:   "#F6FBF9"
                hoverColor:  "#00A1E1"
                editingColor:"#00A1E1"
                minimumLength: 1

                onTextChanged: root.video.name = text

                onCommitted: (value) => {
                    root.video.name = value
                    root.onDataChanged(root.video)
                }
            }

            MyText {
                id: textLabel
                Layout.fillHeight: true
                Layout.fillWidth: false
                Layout.preferredWidth: (root.width - 2 * (10 + root.border.width)) * 0.40
                Layout.maximumWidth: (root.width - 2 * (10 + root.border.width)) * 0.40
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
            Layout.preferredHeight: root.height * 0.8

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
                    anchors.margins: parent.width * 0.05
                    spinnerInnerText: "0%"
                    spinnerInnerSize: 75
                }
            }

            Image {
                id: image
                Layout.fillHeight: true
                Layout.preferredWidth: Layout.preferredHeight
                Layout.maximumWidth: implicitWidth
                Layout.maximumHeight: implicitHeight
                fillMode: Image.Stretch
                source: "qrc:/images/Snow.jpg"

                Layout.rightMargin: root.selected ? 3 : 0
                Layout.leftMargin: root.selected ? 3 : 0
            }
        }

        RowLayout {
            id: footerRow
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredHeight: root.height * 0.1

            Layout.margins: 10 + root.border.width
            Layout.bottomMargin: 5 + root.border.width
            Layout.topMargin: 5 + root.border.width

            Image {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumWidth: footerRow.height
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