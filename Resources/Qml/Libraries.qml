import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Rectangle {
    property string qtVersion: "Unknown"
    property string ffmpegVersion: "Unknown"
    property string ffprobeVersion: "Unknown"

    color: "lightgrey"

    ColumnLayout {
        width: parent.width
        height: parent.height

        GridLayout {
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
            Layout.fillHeight: true
            Layout.fillWidth: true
            columns: 2

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: 50
                Layout.maximumWidth: 200
                Layout.alignment: Qt.AlignTop
                border.width: 3
                border.color: "green"

                Text {
                    Layout.fillWidth: true
                    font.pointSize: 20
                    minimumPointSize: 10
                    fontSizeMode: Text.Fit
                    font.family: "Arial"
                    text: "Qt"
                    padding: 5
                }
            }


            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: 50
                Layout.alignment: Qt.AlignTop
                border.width: 3
                border.color: "green"

                Text {
                    Layout.fillWidth: true
                    font.pointSize: 20
                    minimumPointSize: 10
                    fontSizeMode: Text.Fit
                    font.family: "Arial"
                    text: qtVersion
                    padding: 5
                }

            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: 50
                Layout.alignment: Qt.AlignTop
                Layout.maximumWidth: 200
                border.width: 3
                border.color: "green"

                Text {
                    Layout.fillWidth: true
                    font.pointSize: 20
                    minimumPointSize: 10
                    fontSizeMode: Text.Fit
                    font.family: "Arial"
                    text: "FFmpeg version"
                    padding: 5
                }
            }


            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: 50
                Layout.alignment: Qt.AlignTop
                border.color: "green"
                border.width: 3

                Text {
                    Layout.fillWidth: true
                    font.pointSize: 20
                    minimumPointSize: 10
                    fontSizeMode: Text.Fit
                    font.family: "Arial"
                    text: ffmpegVersion
                    padding: 5
                }

            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: 50
                Layout.alignment: Qt.AlignTop
                Layout.maximumWidth: 200
                border.width: 3
                border.color: "green"

                Text {
                    Layout.fillWidth: true
                    font.pointSize: 20
                    minimumPointSize: 10
                    fontSizeMode: Text.Fit
                    font.family: "Arial"
                    text: "FFprobe version"
                    padding: 5
                }
            }


            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: 50
                Layout.alignment: Qt.AlignTop
                border.color: "green"
                border.width: 3

                Text {
                    Layout.fillWidth: true
                    font.pointSize: 20
                    minimumPointSize: 10
                    fontSizeMode: Text.Fit
                    font.family: "Arial"
                    text: ffprobeVersion
                    padding: 5
                }

            }
        }
    }
}
