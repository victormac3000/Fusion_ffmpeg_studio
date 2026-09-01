import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Rectangle {
    property string compilationDate: "Unknown"
    property string compilationType: "Unknown"
    property string compilationOSName: "Unknown"
    property string compilationOSVersion: "Unknown"

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
                    text: "Compilation date"
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
                    text: compilationDate
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
                    text: "Compilation type"
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
                    text: compilationType
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
                    text: "Compilation OS"
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
                    text: compilationOSName + " " + compilationOSVersion
                    padding: 5
                }

            }
        }
    }
}
