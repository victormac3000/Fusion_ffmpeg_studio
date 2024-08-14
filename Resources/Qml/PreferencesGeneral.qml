import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    color: "lightgrey"
    width: 800
    height: 600

    property string appDataPath: "/Users/victor/Library/Application Support/Victor Corporation/Fusion FFmpeg Studio"
    property string defaultProjectName: ""

    ColumnLayout {
        width: parent.width
        height: parent.height

        GridLayout {
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
            Layout.fillHeight: true
            Layout.fillWidth: true
            columns: 3

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.maximumHeight: 100
                border.width: 3
                border.color: "green"

                ColumnLayout {
                    width: parent.width
                    height: parent.height

                    Text {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        font.pointSize: 16 * constants.fontSizeScale
                        font.family: "Arial"
                        text: qsTr("Application data path")
                        topPadding: 10
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                    }

                    Rectangle {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.minimumHeight: 3
                        Layout.maximumHeight: 3
                        color: "blue"
                    }

                    Text {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignBottom
                        font.pointSize: 16 * constants.fontSizeScale
                        font.family: "Arial"
                        wrapMode: Text.WordWrap
                        bottomPadding: 10
                        leftPadding: 10
                        rightPadding: 10
                        text: qsTr("This folder contains the logs of the application and temporary files")
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.maximumHeight: 100
                border.color: "green"
                border.width: 3

                ColumnLayout {
                    width: parent.width
                    height: parent.height

                    ScrollView {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.margins: 10
                        contentHeight: parent.height

                        TextArea {
                            font.pointSize: 16 * constants.fontSizeScale
                            clip: true
                            font.family: "Arial"
                            text: appDataPath
                            wrapMode: Text.WordWrap
                            readOnly: true
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignTop
                            padding: 5
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.maximumHeight: 100
                Layout.maximumWidth: 100
                border.width: 3
                border.color: "green"

                ColumnLayout {
                    height: parent.height
                    width: parent.width

                    MyButton {
                        id: appDataPathBrowseButton
                        objectName: "appDataPathBrowseButton"
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.maximumHeight: 50
                        Layout.maximumWidth: 100
                        Layout.margins: 5
                        Layout.alignment: Qt.AlignCenter
                        text: qsTr("Browse")
                        //font.pointSize: font.pointSize * constants.fontSizeScale
                    }
                }


            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.maximumHeight: 50
                border.width: 3
                border.color: "green"

                Text {
                    anchors.fill: parent
                    font.pointSize: 16 * constants.fontSizeScale
                    font.family: "Arial"
                    text: qsTr("Default project name")
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    padding: 5
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: 50
                Layout.alignment: Qt.AlignTop
                Layout.columnSpan: 2
                border.width: 3
                border.color: "green"

                TextField {
                    anchors.fill: parent
                    anchors.margins: 5
                    font.pointSize: 16 * constants.fontSizeScale
                    font.family: "Arial"
                    hoverEnabled: true
                    placeholderText: "Example_Project"
                    placeholderTextColor: "grey"
                    text: defaultProjectName
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.maximumHeight: 70
                border.width: 3
                border.color: "green"

                Text {
                    anchors.fill: parent
                    font.pointSize: 16 * constants.fontSizeScale
                    font.family: "Arial"
                    text: qsTr("Font size scale")
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    padding: 5
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.maximumHeight: 70
                Layout.columnSpan: 2
                border.width: 3
                border.color: "green"


                RowLayout {
                    width: parent.width
                    height: parent.height
                    Layout.margins: 10


                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignTop
                        Layout.maximumHeight: fontSizeScaleSlider.height
                        Layout.margins: 10

                        Text {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            font.pointSize: 16 * constants.fontSizeScale
                            font.family: "Arial"
                            text: fontSizeScaleSlider.from
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }


                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.margins: 10

                        Slider {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            id: fontSizeScaleSlider
                            objectName: "fontSizeScaleSlider"
                            from: 0.5
                            value: 1.0
                            to: 3.0
                            stepSize: 0.1

                            onMoved: {
                                constants.fontSizeScale = value
                            }
                        }

                        Text {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.alignment: Qt.AlignTop
                            font.pointSize: 16 * constants.fontSizeScale
                            font.family: "Arial"
                            text: fontSizeScaleSlider.value
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }


                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignTop
                        Layout.maximumHeight: fontSizeScaleSlider.height
                        Layout.margins: 10

                        Text {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            font.pointSize: 16 * constants.fontSizeScale
                            font.family: "Arial"
                            text: fontSizeScaleSlider.to
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                }

            }
        }
    }
}
