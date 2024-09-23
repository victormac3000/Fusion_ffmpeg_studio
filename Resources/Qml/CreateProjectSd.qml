import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Rectangle {
    id: root
    width: 400
    height: 300
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
                text: "SELECT SD CARDS"
                font.pointSize: 16 * constants.fontSizeScale
            }
        }

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 5

            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true
                radius: 15

                ColumnLayout {
                    anchors.fill: parent

                    Rectangle {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.maximumHeight: 40
                        color: "white"

                        Text {
                            anchors.fill: parent
                            text: qsTr("FRONT SD CARD")
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 16 * constants.fontSizeScale
                        }
                    }


                    Rectangle {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        color: valid ? "green" : "red"
                        radius: 15
                        objectName: "frontSDRectangle"
                        property bool valid: false

                        Image {
                            anchors.fill: parent
                            source: "Images/sd-card.png"
                            fillMode: Image.PreserveAspectFit
                        }
                    }

                    Rectangle {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.maximumHeight: 40
                        color: "white"

                        MyComboBox {
                            objectName: "frontSDComboBox"
                            anchors.fill: parent
                            anchors.topMargin: 10

                            signal optionChanged(text: string)

                            onCurrentIndexChanged: {
                                optionChanged(model.get(currentIndex).text)
                            }
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true
                radius: 15

                ColumnLayout {
                    anchors.fill: parent

                    Rectangle {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.maximumHeight: 40
                        color: "white"

                        Text {
                            anchors.fill: parent
                            text: qsTr("BACK SD CARD")
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 16 * constants.fontSizeScale
                        }
                    }


                    Rectangle {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        color: valid ? "green" : "red"
                        radius: 15
                        objectName: "backSDRectangle"
                        property bool valid: false

                        Image {
                            anchors.fill: parent
                            source: "Images/sd-card.png"
                            fillMode: Image.PreserveAspectFit
                        }
                    }

                    Rectangle {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.maximumHeight: 40
                        color: "white"

                        MyComboBox {
                            objectName: "backSDComboBox"
                            anchors.fill: parent
                            anchors.topMargin: 10

                            signal optionChanged(text: string)

                            onCurrentIndexChanged: {
                                optionChanged(model.get(currentIndex).text)
                            }
                        }
                    }
                }
            }
        }



        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumWidth: 30
            Layout.maximumHeight: 40
            Layout.margins: 5

            MyButton {
                objectName: "createProjectSDBackButton"
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: qsTr("Back")
            }

            MyButton {
                objectName: "createProjectSDButton"
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: qsTr("Create project")
                enabled: false
            }

        }
    }

}
