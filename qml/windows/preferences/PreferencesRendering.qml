import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import FusionFFmpegStudio

Rectangle {
    id: root
    color: "lightgrey"

    required property var paneController

    ColumnLayout {
        width: parent.width
        height: parent.height

        GridLayout {
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
            Layout.fillHeight: true
            Layout.fillWidth: true
            columns: 4

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.maximumWidth: parent.width*0.15

                border.color: "green"
                border.width: 3

                Text {
                    anchors.fill: parent
                    clip: true
                    text: "Codec"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    padding: 5
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.minimumHeight: 100
                Layout.columnSpan: 2
                border.color: "green"
                border.width: 3

                ScrollView {
                    anchors.fill: parent
                    anchors.margins: 10
                    contentHeight: codecsDescriptionText.height

                    TextArea {
                        id: codecsDescriptionText
                        clip: true
                        text: ""
                        wrapMode: Text.WordWrap
                        readOnly: true
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignTop
                        padding: 5
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.maximumWidth: parent.width*0.25
                border.color: "green"
                border.width: 3

                MyComboBox {
                    id: codecsComboBox
                    anchors.fill: parent
                    anchors.margins: 10

                    onCurrentIndexChanged: {
                        // TODO
                    }
                }
            }


            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.maximumWidth: parent.width*0.15
                border.color: "green"
                border.width: 3

                Text {
                    anchors.fill: parent
                    text: "Encoder"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    padding: 5
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.minimumHeight: 100
                border.color: "green"
                border.width: 3

                ScrollView {
                    anchors.fill: parent
                    anchors.margins: 10
                    contentHeight: encodersDescriptionText.height

                    TextArea {
                        id: encodersDescriptionText
                        clip: true
                        text: ""
                        readOnly: true
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignTop
                        padding: 5
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.minimumHeight: 100
                border.color: "green"
                border.width: 3

                Image {
                    id: acceleratorImage
                    source: hardware ? "qrc:/images/gpu_icon.png" : "qrc:/images/cpu_icon.png"
                    fillMode: Image.PreserveAspectFit
                    width: parent.width
                    height: parent.height
                    property bool hardware: false
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.maximumWidth: parent.width*0.25
                border.color: "green"
                border.width: 3

                MyComboBox {
                    id: encodersComboBox
                    anchors.fill: parent
                    anchors.margins: 10

                    onCurrentIndexChanged: {
                        // TODO
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.maximumWidth: parent.width*0.15
                border.color: "green"
                border.width: 3

                Text {
                    anchors.fill: parent
                    text: "Format"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    padding: 5
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.minimumHeight: 100
                Layout.columnSpan: 2
                border.color: "green"
                border.width: 3

                ScrollView {
                    anchors.fill: parent
                    anchors.margins: 10
                    contentHeight: formatsDescriptionText.height

                    TextArea {
                        id: formatsDescriptionText
                        clip: true
                        text: ""
                        readOnly: true
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignTop
                        padding: 5
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.maximumWidth: parent.width*0.25
                border.color: "green"
                border.width: 3

                MyComboBox {
                    id: formatsComboBox
                    anchors.fill: parent
                    anchors.margins: 10

                    onCurrentIndexChanged: {
                        // TODO
                    }
                }
            }
        }
    }
}
