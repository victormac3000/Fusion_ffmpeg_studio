import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Rectangle {
    color: "lightgrey"
    id: root

    required property var paneController
    readonly property var ffmpegLibs: paneController.getFFmpegVersions()
    readonly property string qtVersion: paneController.getQtVersion()

    ScrollView {
        anchors.fill: parent

        ColumnLayout {
            id: itemsLayout
            width: root.width
            height: root.height

            BuildItem {
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                name: "Qt framework"
                value: qtVersion
            }

            BuildItem {
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                name: "FFmpeg"
                value: ffmpegLibs["FFmpeg"]
            }

            BuildItem {
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                name: "FFmpeg (avcodec)"
                value: ffmpegLibs["avcodec"]
            }

            BuildItem {
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                name: "FFmpeg (avformat)"
                value: ffmpegLibs["avformat"]
            }

            BuildItem {
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                name: "FFmpeg (avutil)"
                value: ffmpegLibs["avutil"]
            }

            BuildItem {
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                name: "FFmpeg (swresample)"
                value: ffmpegLibs["swresample"]
            }

            BuildItem {
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                name: "FFmpeg (swscale)"
                value: ffmpegLibs["swscale"]
            }

            // Used for compacting the BuildItem components
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "transparent"
            }
        }
    }
}