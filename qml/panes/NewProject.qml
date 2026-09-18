import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import QtQuick.Dialogs

import FusionFFmpegStudio

Rectangle {
    id: root
    color: "#69ff00"

    property int generalMargin: 10

    NewProjectController {
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
                text: qsTr("CREATE PROJECT")
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
                                projectPathTextField.text = mergedPath
                            },
                            (error) => {
                                state = "error"
                                stateText = error
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
                                    stateText = "Path changed to avoid duplicate folder"
                                    state = "warning"
                                } else {
                                    state = "normal"
                                }
                            },
                            (error) => {
                                stateText = error
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
                    text: "PROJECT SOURCE"
                }

                TabBar {
                    id: sourceTabBar
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30

                    background: Rectangle {
                        color: parent.palette.window
                    }

                    TabButton {
                        text: "FROM SD CARDS"
                        padding: 0
                        implicitHeight: parent.height
                        implicitWidth: parent.width/2
                    }

                    TabButton {
                        text: "FROM DCIM FOLDER"
                        padding: 0
                        implicitHeight: parent.height
                        implicitWidth: parent.width/2
                    }
                }
            }

            RowLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.maximumHeight: 30
                visible: sourceTabBar.currentIndex === 1

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

            RowLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.maximumHeight: 30
                visible: sourceTabBar.currentIndex === 1

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
            }


            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: 30
                visible: sourceTabBar.currentIndex === 0

                Text {
                    Layout.maximumWidth: 150
                    Layout.minimumWidth: 150
                    Layout.fillWidth: true
                    text: "FRONT SD CARD"
                }

                MyComboBox {
                    id: frontSDComboBox
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    placeholderText: "-Select SD card that contains the front source files-"

                    onCurrentIndexChanged: {
                        var item = model.get(currentIndex)
                        if (!item) return
                        if (item.frontCandidate) {
                            state = "normal"
                        } else {
                            stateText = "GoPro front SD source files not found on this device"
                            state = "error"
                        }
                    }

                    function reload() {
                        state = "normal"
                        model.clear()

                        var externalVolumes = controller.getExternalVolumes()

                        externalVolumes.forEach(function (volumeInfo) {
                            var text = volumeInfo["label"]
                                    + " (" + volumeInfo["mountPath"] + ") "
                                    + "[" + volumeInfo["deviceName"] + "]"

                            var candidate = volumeInfo["frontCandidate"] === true

                            model.append({
                                text: text,
                                frontCandidate: candidate,
                                volumeInfo: volumeInfo
                            })

                            if (candidate) {
                                currentIndex = model.count - 1
                            }
                        })
                    }

                    Component.onCompleted: reload()
                }

                MyButton {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.maximumWidth: 100
                    Layout.minimumWidth: 100
                    text: qsTr("Refresh")

                    onClicked: frontSDComboBox.reload()
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: 30
                visible: sourceTabBar.currentIndex === 0

                Text {
                    Layout.maximumWidth: 150
                    Layout.minimumWidth: 150
                    Layout.fillWidth: true
                    text: "BACK SD CARD"
                }

                MyComboBox {
                    id: backSDComboBox
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    placeholderText: "-Select SD card that contains the front back files-"

                    onCurrentIndexChanged: {
                        var item = model.get(currentIndex)
                        if (!item) return
                        if (item.backCandidate) {
                            state = "normal"
                        } else {
                            stateText = "GoPro front SD back files not found on this device"
                            state = "error"
                        }
                    }

                    function reload() {
                        state = "normal"
                        model.clear()

                        var externalVolumes = controller.getExternalVolumes()

                        externalVolumes.forEach(function (volumeInfo) {
                            var text = volumeInfo["label"]
                                    + " (" + volumeInfo["mountPath"] + ") "
                                    + "[" + volumeInfo["deviceName"] + "]"

                            var candidate = volumeInfo["backCandidate"] === true

                            model.append({
                                text: text,
                                backCandidate: candidate,
                                volumeInfo: volumeInfo
                            })

                            if (candidate) {
                                currentIndex = model.count - 1
                            }
                        })
                    }

                    Component.onCompleted: reload()
                }

                MyButton {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.maximumWidth: 100
                    Layout.minimumWidth: 100
                    text: qsTr("Refresh")

                    onClicked: backSDComboBox.reload()
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
                Layout.minimumHeight: 30

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
                        var ok = projectNameTextField.state !== "error" &&
                                projectPathTextField.state !== "error"

                        // Create project from DCIM folder
                        if (sourceTabBar.currentIndex === 0) {
                            ok &= frontSDComboBox.state !== "error" &&
                                    frontSDComboBox.currentIndex >= 0 &&
                                    backSDComboBox.state !== "error" &&
                                    backSDComboBox.currentIndex >= 0
                        }

                        // Create project from SD cards
                        if (sourceTabBar.currentIndex === 1) {
                            ok &= dcimPathTextField.text !== ""
                        }

                        return ok
                    }

                    onClicked: {
                        var args = {
                            projectName: projectNameTextField.text,
                            projectPath: projectPathTextField.text
                        }

                        if (sourceTabBar.currentIndex === 0) {
                            args.operation = "CREATE_PROJECT_SD"
                            args.frontPath = frontSDComboBox.model.get(frontSDComboBox.currentIndex).volumeInfo["mountPath"]
                            args.backPath = backSDComboBox.model.get(backSDComboBox.currentIndex).volumeInfo["mountPath"]
                        }

                        if (sourceTabBar.currentIndex === 1) {
                            args.operation = "CREATE_PROJECT_DCIM"
                            args.dcimPath = dcimPathTextField.text
                            args.dcimCopy = copyCheckbox.checked
                        }

                        controller.createProject(
                            args,
                            () => {},
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