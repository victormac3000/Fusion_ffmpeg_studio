import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

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
                    onClicked: controller.onLoadProjectClicked()
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
                    console.log("Text changed: " + text)
                    //recentProjectsLayout.filter(text)
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
                                controller.loadRecentProject(args, () => {
                                    projectLoading = false
                                }, (error) => {
                                    dialogs.error("Could not load project", "Error loading project", () => {
                                        projectLoading = false
                                    })
                                })
                            }

                            function addRecentProject(recentProject) {
                                var uuid = recentProject.uuid
                                var name = recentProject.name
                                var path = recentProject.path

                                var recentProjectComponent = Qt.createComponent("RecentProject.qml")
                                if (recentProjectComponent.status === Component.Ready) {
                                    var recentProjectElement = recentProjectComponent.createObject(recentProjectsLayout)
                                    if (recentProjectElement !== null) {
                                        recentProjectElement.uuid = uuid
                                        recentProjectElement.name = name
                                        recentProjectElement.path = path
                                        recentProjectElement.anchors.margins = 5
                                        recentProjectElement.Layout.fillWidth = true
                                        recentProjectElement.Layout.minimumHeight = 100
                                        recentProjectElement.Layout.maximumHeight = 100
                                        recentProjectElement.clickHandler = onRecentProjectClicked
                                    } else {
                                        dialogs.warning("Failed to load a recent project")
                                        console.error("Failed to create RecentProject element")
                                    }
                                } else {
                                    dialogs.warning("Failed to load a recent project")
                                    console.error("Failed to load RecentProject.qml:", recentProjectComponent.errorString())
                                }
                            }

                            /*
                            function filter(name) {
                                for (let i=0; i<children.length; i++) {
                                    let child = children[i]
                                    children[i].visible = child.name.toLowerCase().includes(name.toLowerCase())
                                }
                            }
                            */
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
