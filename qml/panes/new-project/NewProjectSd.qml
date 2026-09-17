import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import QtQuick.Dialogs

import FusionFFmpegStudio

Rectangle {
    id: root
    color: "lightblue"

    NewProjectSdController {
        id: controller
        appController: mainController
    }

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
            Layout.maximumHeight: 30
            Layout.margins: 5

            MyTextField {
                id: projectNameTextField
                Layout.fillHeight: true
                Layout.fillWidth: true
                placeholderText: qsTr("Project name")
                placeholderTextColor: "grey"
                text: controller.getDefaultProjectName()

                onTextChanged: {
                    var args = {
                        projectName: projectNameTextField.text,
                        projectPath: projectPathTextField.basePath
                    }
                    controller.validateProjectName(args,
                        (mergedPath) => {
                            state = "normal"
                            errorText = ""
                            projectPathTextField.text = mergedPath
                        },
                        (error) => {
                            state = "error"
                            errorText = error
                            projectPathTextField.text = projectPathTextField.basePath
                        }
                    )
                }
            }
        }


        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 30
            Layout.margins: 5

            MyTextField {
                id: projectPathTextField
                Layout.fillHeight: true
                Layout.fillWidth: true
                placeholderText: qsTr("Project path")
                placeholderTextColor: "grey"
                readOnly: true

                property string basePath: controller.getDefaultProjectPath()

                onTextChanged: {
                    if (basePath === text) {
                        state = "normal"
                        return
                    }
                    var args = {
                        projectBasePath: basePath,
                        projectName: projectNameTextField.text
                    }
                    controller.validateProjectPath(
                        args,
                        (pathDiffersName) => {
                            if (pathDiffersName) {
                                warningText = "Path changed to avoid duplicate folder"
                                state = "warning"
                            } else {
                                state = "normal"
                            }
                        },
                        (error) => {
                            errorText = error
                            state = "error"
                        }
                    )
                }
            }

            MyButton {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.minimumWidth: 30
                Layout.maximumHeight: 30
                text: qsTr("Browse")

                onClicked: {
                    browseProjectPathDialog.open()
                }

                FolderDialog {
                    id: browseProjectPathDialog
                    onAccepted: {
                        var args = {
                            projectName: projectNameTextField.text,
                            selectedFolder: selectedFolder
                        }
                        controller.generateProjectPath(
                            args,
                            (newBaseFolder, newProjectPath) => {
                                projectPathTextField.basePath = newBaseFolder
                                projectPathTextField.text = newProjectPath
                            },
                            (error) => {
                                dialogs.warning(error)
                            }
                        )
                    }
                }
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
                        property bool valid: false

                        Image {
                            anchors.fill: parent
                            source: "qrc:/images/sd-card.png"
                            fillMode: Image.PreserveAspectFit
                        }
                    }

                    Rectangle {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.maximumHeight: 40
                        color: "white"

                        MyComboBox {
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
                        property bool valid: false

                        Image {
                            anchors.fill: parent
                            source: "qrc:/images/sd-card.png"
                            fillMode: Image.PreserveAspectFit
                        }
                    }

                    Rectangle {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.maximumHeight: 40
                        color: "white"

                        MyComboBox {
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
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: qsTr("Back")
                onClicked: {
                    mainController.back()
                }
            }

            MyButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: qsTr("Create project")
                enabled: false
            }

        }
    }

}
