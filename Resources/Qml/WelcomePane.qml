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

            RowLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.maximumHeight: 50
                Layout.margins: 10


                Button {
                    objectName: "settingsButton"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignLeft
                    Text {
                        anchors.fill: parent
                        font.pointSize: 600
                        minimumPointSize: 10
                        fontSizeMode: Text.Fit
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: "Settings"
                        color: "white"
                    }
                }

                Button {
                    objectName: "loadProjectButton"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignRight
                    Text {
                        anchors.fill: parent
                        font.pointSize: 600
                        minimumPointSize: 10
                        fontSizeMode: Text.Fit
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: "Load project"
                        color: "white"
                    }
                }

                Button {
                    objectName: "newProjectButton"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignRight
                    Text {
                        anchors.fill: parent
                        font.pointSize: 600
                        minimumPointSize: 10
                        fontSizeMode: Text.Fit
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: "New project"
                        color: "white"
                    }
                }

            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 4
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

                }
            }

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: 10
            }

            Flickable {
                Layout.fillWidth: true
                Layout.fillHeight: true
                flickableDirection: Flickable.VerticalFlick
                boundsBehavior: Flickable.DragOverBounds
                clip: true
                contentHeight: recentProjectsLayout.height


                ColumnLayout {
                    id: recentProjectsLayout
                    objectName: "recentProjectsLayout"
                    width: parent.width

                    // TODO: Project search

                    Component.onCompleted: {
                        /*
                        for (var i=0; i<10; i++) {
                            addRecentProject("Project", "/test/x.ffs")
                        }*/
                    }

                    function addRecentProject(name, path) {
                        var recentProjectComponent = Qt.createComponent("RecentProject.qml")
                        var recentProjectElement = recentProjectComponent.createObject(recentProjectsLayout)
                        recentProjectElement.name = name
                        recentProjectElement.path = path
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
