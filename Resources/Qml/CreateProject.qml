import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Rectangle {
    id: root
    width: 600
    height: 400
    color: "lightblue"

    Constants {
        id: constants
        visible: false
    }

    ColumnLayout {
        anchors.fill: parent

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 50
            Layout.margins: 5

            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "SELECT IMPORT METHOD"
                font.pointSize: 16 * constants.fontSizeScale
            }
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 5
            color: "black"
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 5
            radius: 15

            RowLayout {
                anchors.fill: parent

                Rectangle {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    radius: 15
                    color: "#3E3EFA"

                    MouseArea {
                        objectName: "sdCardMouseArea"
                        anchors.fill: parent
                        hoverEnabled: true
                        signal clickDetected()

                        onClicked: {
                            clickDetected()
                        }

                        onEntered: {
                            parent.color = "#8B8BFF"
                        }
                        onExited: {
                            parent.color = "#3E3EFA"
                        }
                    }

                    Image {
                        anchors.fill: parent
                        source: "Images/sd-card.png"
                        fillMode: Image.PreserveAspectFit
                    }
                }

                Rectangle {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    radius: 15
                    color: "#3E3EFA"

                    MouseArea {
                        objectName: "dcimFolderMouseArea"
                        anchors.fill: parent
                        hoverEnabled: true
                        signal clickDetected()

                        onClicked: {
                            clickDetected()
                        }

                        onEntered: {
                            parent.color = "#8B8BFF"
                        }
                        onExited: {
                            parent.color = "#3E3EFA"
                        }
                    }

                    Image {
                        anchors.fill: parent
                        source: "Images/folder.png"
                        fillMode: Image.PreserveAspectFit

                        Text {
                            anchors.fill: parent
                            text: "DCIM\nFOLDER"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            topPadding: 60
                            font.pointSize: 20
                        }
                    }
                }

            }

        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 50
            Layout.margins: 5
            radius: 15

            MyButton {
                objectName: "createProjectBackButton"
                anchors.fill: parent
                text: qsTr("Back")
            }

        }


    }

}
