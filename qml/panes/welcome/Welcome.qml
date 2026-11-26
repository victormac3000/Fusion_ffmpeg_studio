import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import QtQuick.Dialogs

import FusionFFmpegStudio

Rectangle {
    id: root

    WelcomeController {
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

    FolderDialog {
        id: loadProjectDialog

        onAccepted: {
            var args = {
                operation: constants.loadProject,
                projectPath: selectedFolder
            }
            controller.loadProject(args, () => {},
                (error) => {
                    dialogs.warning(error, qsTr("Error loading project"), () => {
                })
            })
        }
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 60
            Layout.margins: 10

            RowLayout {
                Layout.fillHeight: true

                Image {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.maximumWidth: 60
                    source: "qrc:/images/AppIcon.png"
                }

                ColumnLayout {
                    Layout.fillHeight: true

                    Text {
                        objectName: "appTitleLabel"
                        Layout.fillHeight: true
                        text: "Fusion FFmpeg Studio"
                        font.pointSize: 20
                        minimumPointSize: 10
                        fontSizeMode: Text.Fit
                        font.family: "Arial"
                    }
                    Text {
                        objectName: "appVersionLabel"
                        Layout.fillHeight: true
                        text: "0.0.1"
                        font.pointSize: 20
                        minimumPointSize: 10
                        fontSizeMode: Text.Fit
                    }
                }
            }

            RowLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.maximumHeight: 50
                Layout.margins: 10

                MyButton {
                    objectName: "loadProjectButton"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignRight
                    text: "Load project"
                    onClicked: {
                        loadProjectDialog.open()
                    }
                }

                MyButton {
                    objectName: "newProjectButton"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignRight
                    text: "New project"
                    onClicked: controller.onNewProjectClicked()
                }

            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 2
            color: "black"
        }

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 10

            TextField {
                font.pointSize: 20
                font.family: "Arial"
                hoverEnabled: true
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: 35
                placeholderText: "Search recent projects"
                placeholderTextColor: "grey"

                onTextChanged: {
                    recentProjectsLayout.filter(text)
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: 10
            }

            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Rectangle {
                    id: borderRectangle
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    border.width: 2
                    border.color: "black"
                    color: "transparent"

                    Flickable {
                        id: flickable
                        anchors.fill: parent
                        anchors.margins: borderRectangle.border.width
                        flickableDirection: Flickable.VerticalFlick
                        boundsBehavior: Flickable.DragOverBounds
                        clip: true
                        contentHeight: recentProjectsLayout.height

                        ColumnLayout {
                            id: recentProjectsLayout
                            width: parent.width
                            property bool projectLoading: false

                            Component.onCompleted: {
                                reloadRecentProjects()
                            }

                            function filter(name) {
                                recentProjectsLayout.children.forEach((child) => {
                                    if (name.trim() === '') {
                                        child.visible = true
                                        return
                                    }
                                    child.visible = child.name.toLowerCase().includes(name.toLowerCase())
                                })
                            }

                            function reloadRecentProjects() {
                                // Clear the layout
                                for (let i = recentProjectsLayout.children.length - 1; i >= 0; i--) {
                                    recentProjectsLayout.children[i].destroy()
                                }

                                controller.getRecentProjects({}, (recentProjects) => {
                                    borderRectangle.color = (recentProjects.length < 1) ? "lightgrey" : "transparent"

                                    recentProjects.forEach((recentProject) => {
                                        addRecentProject(recentProject)
                                    })
                                }, (error) => {
                                    dialogs.warning(error)
                                })
                            }

                            function onRecentProjectClicked(uuid, name, path) {
                                if (projectLoading) return
                                var args = {
                                    operation: constants.loadProject,
                                    projectName: name,
                                    projectPath: path,
                                    projectUUID: uuid
                                }
                                controller.loadProject(args, () => {
                                    projectLoading = false
                                }, (error) => {
                                    console.error("Could not load project", error)
                                    dialogs.warning(qsTr("Could not load project"), qsTr("Error loading project"), () => {
                                        projectLoading = false
                                    })
                                })
                            }

                            function onDeleteRecentProjectClicked(uuid) {
                                var args = {
                                    uuid: uuid
                                }
                                controller.deleteRecentProject(args, () => {
                                    reloadRecentProjects()
                                }, (error) => {
                                    dialogs.error(error, qsTr("Error removing recent project"))
                                })
                            }

                            function addRecentProject(recentProject) {
                                var uuid = recentProject.uuid
                                var name = recentProject.name
                                var path = recentProject.path
                                var enabled = recentProject.enabled

                                var recentProjectComponent = Qt.createComponent("RecentProject.qml")
                                if (recentProjectComponent.status === Component.Ready) {
                                    var recentProjectElement = recentProjectComponent.createObject(recentProjectsLayout)
                                    if (recentProjectElement !== null) {
                                        recentProjectElement.uuid = uuid
                                        recentProjectElement.name = name
                                        recentProjectElement.path = path
                                        recentProjectElement.valid = enabled
                                        recentProjectElement.anchors.margins = 5
                                        recentProjectElement.Layout.fillWidth = true
                                        recentProjectElement.Layout.minimumHeight = 100
                                        recentProjectElement.clickHandler = onRecentProjectClicked
                                        recentProjectElement.removeClickHandler = onDeleteRecentProjectClicked
                                    } else {
                                        dialogs.warning("Failed to load a recent project")
                                        console.error("Failed to create RecentProject element")
                                    }
                                } else {
                                    dialogs.warning("Failed to load a recent project")
                                    console.error("Failed to load RecentProject.qml:", recentProjectComponent.errorString())
                                }
                            }
                        }

                        ScrollBar.vertical: ScrollBar {
                            policy: ScrollBar.AsNeeded
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 2
            color: "black"
        }

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 40
            Layout.margins: 10

            MyButton {
                objectName: "aboutButton"
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                text: "About"
                onClicked: controller.onAboutClicked()
            }

            MyButton {
                objectName: "settingsButton"
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                text: "Settings"
                onClicked: controller.onSettingsClicked()
            }
        }
    }
}
