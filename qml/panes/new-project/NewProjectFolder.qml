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
                    borderColor: "red"

                    onTextEdited: {
                        var proposedPath = projectPathTextField.basePath + "/" + text
                        if (text.length > 0 || controller.verifyProjectPath(proposedPath)) {
                            borderColor = "blue"
                            projectPathTextField.text = proposedPath
                        } else {
                            borderColor = "red"
                            projectPathTextField.text = projectPathTextField.basePath
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
                    text: "PROJECT PATH"
                }

                MyTextField {
                    id: projectPathTextField
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    placeholderTextColor: "grey"
                    readOnly: true
                    text: basePath

                    property string basePath: controller.getDefaultProjectPath()
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
                            root.spinner = dialogs.spinner("Changing project path")
                            var args = {"newPath": selectedFolder}
                            controller.validateProjectPath(
                                args,
                                (newPath) => {
                                    projectPathTextField.basePath = newPath
                                    spinner.close()
                                },
                                (error) => {
                                    spinner.close()
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
                            root.spinner = dialogs.spinner("Changing DCIM folder path")
                            var args = {"newPath": selectedFolder}
                            controller.validateDCIMPath(
                                args,
                                (newPath) => {
                                    dcimPathTextField.text = newPath
                                    spinner.close()
                                },
                                (error) => {
                                    spinner.close()
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
                }

            }
        }

    }

}
