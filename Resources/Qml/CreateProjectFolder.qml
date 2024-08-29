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
                text: qsTr("SELECT THE PROJECT FOLDER")
                font.pointSize: 16 * constants.fontSizeScale
            }
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 5
            color: "black"
        }

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 30
            Layout.margins: 5

            TextField {
                objectName: "projectNameField"
                Layout.fillHeight: true
                Layout.fillWidth: true
                placeholderText: qsTr("Project name")
                placeholderTextColor: "grey"
                text: "wrong"
            }
        }



        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 30
            Layout.margins: 5

            TextField {
                objectName: "projectPathField"
                Layout.fillHeight: true
                Layout.fillWidth: true
                placeholderText: qsTr("Project path")
                placeholderTextColor: "grey"
                readOnly: true
            }

            MyButton {
                objectName: "projectPathButton"
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.minimumWidth: 30
                Layout.maximumHeight: 30
                text: qsTr("Browse")
            }
        }

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 30
            Layout.margins: 5

            TextField {
                objectName: "projectDCIMField"
                Layout.fillHeight: true
                Layout.fillWidth: true
                placeholderText: qsTr("DCIM folder path")
                placeholderTextColor: "grey"
                readOnly: true
            }

            MyButton {
                objectName: "projectDCIMButton"
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.minimumWidth: 30
                Layout.maximumHeight: 30
                text: qsTr("Browse")
            }
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 80
            Layout.margins: 5

            GridLayout {
                anchors.fill: parent
                columns: 2

                RadioButton {
                    objectName: "copyDCIMCheckbox"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.minimumWidth: 200
                    text: qsTr("Copy DCIM folder to project")
                }

                Text {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("Makes a copy of the selected DCIM folder")
                }

                RadioButton {
                    objectName: "linkDCIMCheckbox"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    text: qsTr("Link DCIM folder to project")
                    checked: true
                }

                Text {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("References the DCIM folder to use it.\nIf DCIM folder is moved, the project breaks")
                }

            }

        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumWidth: 20
            Layout.maximumHeight: 20
            Layout.margins: 5
            visible: createProjectFolderProgressBar.visible

            NiceProgressBar {
                id: createProjectFolderProgressBar
                objectName: "createProjectFolderProgressBar"
                anchors.fill: parent
                insideText: "0%"
                visible: false
            }
        }


        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumWidth: 30
            Layout.maximumHeight: 30
            Layout.margins: 5

            MyButton {
                objectName: "createProjectFolderBackButton"
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: qsTr("Back")
            }

            MyButton {
                objectName: "createProjectFolderButton"
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: qsTr("Create project")
            }

        }


    }

}
