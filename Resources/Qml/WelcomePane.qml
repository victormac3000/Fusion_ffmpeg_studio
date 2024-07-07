import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Rectangle {
    width: 800
    height: 600

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
                    source: "Images/AppIcon.png"
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
                }

                MyButton {
                    objectName: "newProjectButton"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignRight
                    text: "New project"
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
                            objectName: "recentProjectsLayout"
                            width: parent.width

                            Component.onCompleted: {
/*
                                for (var i = 0; i < 10; i++) {
                                    addRecentProject("Project " + (i + 1), "/test/x" + i + ".ffs")
                                }
*/
                                if (recentProjectsLayout.children.length < 1) {
                                    borderRectangle.color = "lightgrey"
                                } else {
                                    borderRectangle.color = "transparent"

                                }
                            }

                            function filter(name) {
                                for (let i=0; i<children.length; i++) {
                                    let child = children[i]
                                    children[i].visible = child.name.toLowerCase().includes(name.toLowerCase())
                                }
                            }

                            function addRecentProject(name, path) {
                                var recentProjectComponent = Qt.createComponent("RecentProject.qml")
                                if (recentProjectComponent.status === Component.Ready) {
                                    var recentProjectElement = recentProjectComponent.createObject(recentProjectsLayout)
                                    if (recentProjectElement !== null) {
                                        recentProjectElement.name = name
                                        recentProjectElement.path = path
                                        recentProjectElement.anchors.margins = 5
                                    } else {
                                        console.error("Failed to create RecentProject element")
                                    }
                                } else {
                                    console.error("Failed to load RecentProject.qml:", recentProjectComponent.errorString())
                                }
                            }

                            signal recentProjectClicked(var recentProject)
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
            }

            MyButton {
                objectName: "settingsButton"
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                text: "Settings"
            }


        }
    }
}
