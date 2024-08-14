import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    color: "lightgrey"
    width: 800
    height: 600

    property string appDataPath: ""
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
                Layout.maximumHeight: 50
                Layout.alignment: Qt.AlignTop
                border.width: 3
                border.color: "green"

                Text {
                    anchors.fill: parent
                    font.pointSize: 16
                    font.family: "Arial"
                    text: qsTr("Application data path")
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    padding: 5

                    MouseArea {
                        id: appDataPathImageMouseArea
                        anchors.fill: parent
                        hoverEnabled: true

                        onEntered: {
                            appDataPathPopup.open()
                        }

                        onExited: {
                            closePopup()
                        }
                    }

                    Popup {
                        id: appDataPathPopup
                        width: 200
                        height: contentItem.implicitHeight

                        contentItem: Rectangle {
                            id: contentRect
                            color: "grey"
                            implicitWidth: 200
                            implicitHeight: textElement.implicitHeight + 10 // Adding some padding

                            Text {
                                id: textElement
                                anchors.fill: parent
                                font.pointSize: 16
                                font.family: "Arial"
                                text: qsTr("This folder contains the logs of the application and temporary files")
                                wrapMode: Text.WordWrap
                                padding: 5
                            }
                        }
                    }
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
                    anchors.fill: parent
                    font.pointSize: 16
                    font.family: "Arial"
                    text: appDataPath
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
                Layout.maximumWidth: 100
                border.width: 3
                border.color: "green"

                MyButton {
                    id: appDataPathBrowseButton
                    objectName: "appDataPathBrowseButton"
                    anchors.fill: parent
                    text: qsTr("Browse")
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
                    anchors.fill: parent
                    font.pointSize: 16
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
                    font.pointSize: 16
                    font.family: "Arial"
                    hoverEnabled: true
                    placeholderText: "Example_Project"
                    placeholderTextColor: "grey"
                    text: defaultProjectName
                }
            }
        }
    }

    Timer {
        id: timer
        repeat: false

        function delay(delayTime, cb) {
            timer.interval = delayTime
            timer.triggered.connect(cb)
            timer.start()
        }
    }

    function closePopup() {
        timer.delay(100, function() {
            appDataPathPopup.close()
        })
    }
}
