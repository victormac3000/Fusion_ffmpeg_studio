import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import QtQuick.Dialogs

import FusionFFmpegStudio

Rectangle {
    id: root
    color: "#69ff00"

    property int generalMargin: 10
    property var spinner

    NewProjectFolderController {
        id: controller
        appController: mainController
    }

    Dialogs {
        id: dialogs
    }

    Constants {
        id: constants
        visible: false
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 50
            Layout.margins: generalMargin

            Text {
                Layout.fillHeight: true
                Layout.fillWidth: true
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

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: generalMargin

            RowLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Text {
                    Layout.maximumWidth: 150
                    Layout.minimumWidth: 150
                    Layout.fillWidth: true
                    text: "PROJECT NAME"
                }

                MyTextField {
                    id: projectNameTextField
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.maximumHeight: 30
                    placeholderTextColor: "grey"
                    maximumLength: 50
                    placeholderText: "Project name"
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

                Text {
                    Layout.maximumWidth: 150
                    Layout.fillWidth: true
                    Layout.minimumWidth: 150
                    text: "PROJECT PATH"
                }

                MyTextField {
                    id: projectPathTextField
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    placeholderTextColor: "grey"
                    readOnly: true
                    text: basePath
                    placeholderText: "Project path"

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
                        controller.validateProjectpath(
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
                    Layout.maximumWidth: 100
                    Layout.minimumWidth: 100
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
                Layout.maximumHeight: 30

                Text {
                    Layout.maximumWidth: 150
                    Layout.fillWidth: true
                    Layout.minimumWidth: 150
                    text: "DCIM FOLDER PATH"
                }

                MyTextField {
                    id: dcimPathTextField
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    placeholderText: qsTr("DCIM folder path")
                    placeholderTextColor: "grey"
                    readOnly: true
                }

                MyButton {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.maximumWidth: 100
                    Layout.minimumWidth: 100
                    text: qsTr("Browse")

                    onClicked: {
                        browseDCIMPathDialog.open()
                    }

                    FolderDialog {
                        id: browseDCIMPathDialog
                        onAccepted: {
                            var args = {
                                dcimPath: selectedFolder
                            }
                            controller.validateDCIMPath(
                                args,
                                (dcimPath) => {
                                    dcimPathTextField.text = dcimPath
                                },
                                (error) => {
                                    dialogs.warning(error)
                                }
                            )
                        }
                    }
                }
            }

            CheckBox {
                id: copyCheckbox
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.maximumHeight: 30
                text: "Copy videos into the project"
                checked: true
                onToggled: {
                    if (!checked) {
                        checked = true
                        dialogs.questionw(
                            "Remember that the project will require the source files are not moved.\n" +
                            "If you move the files the project won't work anymore",
                            "Copy videos",
                            (res) => {
                                if (res["button"] === "YES") {
                                    copyCheckbox.checked = false
                                }
                            }
                        )
                    }
                }
            }

            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: "transparent"
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: 30

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

                    enabled: {
                        return projectNameTextField.state !== "error" &&
                            projectPathTextField.state !== "error" &&
                            dcimPathTextField.text !== ""
                    }

                    onClicked: {
                        var args = {
                            projectName: projectNameTextField.text,
                            projectPath: projectPathTextField.text,
                            dcimPath: dcimPathTextField.text,
                            dcimCopy: copyCheckbox.checked
                        }
                        controller.createProject(
                            args,
                            (projectName, projectPath, dcimPath, dcimCopy) => {
                                console.log("projectName=" + projectName)
                            },
                            (error) => {
                                dialogs.warning(error)
                            }
                        )
                    }
                }

            }
        }

    }

}
