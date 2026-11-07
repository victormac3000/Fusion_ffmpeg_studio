import QtQuick
import QtQuick.Layouts

import FusionFFmpegStudio

Rectangle {
    id: root
    color: "#000000"

    required property var video

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 10

            MyText {
                Layout.fillHeight: true
                Layout.fillWidth: true
                text: qsTr("VIDEO") + " " + root.video.idString
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            MyText {
                Layout.fillHeight: true
                Layout.fillWidth: true
                text: root.video.dateString
            }
        }

        Image {
            Layout.fillHeight: true
            Layout.fillWidth: true
            fillMode: Image.Stretch
            source: "qrc:/images/Snow.jpg"
        }

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 10

            Image {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.maximumHeight: parent.height
                Layout.maximumWidth: parent.height
                Layout.rightMargin: 10
                fillMode: Image.PreserveAspectFit
                source: "qrc:/icons/editor/picture-frame-white.png"
            }

            MyText {
                Layout.fillHeight: true
                Layout.fillWidth: true
                text: root.video.durationString
            }
        }
    }
}
