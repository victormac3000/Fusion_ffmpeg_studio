import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs

import FusionFFmpegStudio

Rectangle {
    id: root
    color: "lightgrey"

    required property var parentWindow
    required property var paneController
    property string appDataPath: paneController.getAppDataPath()
    property string defaultProjectName: paneController.getDefaultProjectName()

    property var spinner

    Dialogs {
        id: dialogs
    }

    Connections {
        target: paneController

        function onAppDataPathChanged(retVal) {
            root.spinner.close()
            if (!retVal["ok"]) {
                dialogs.warning(retVal["error"], "")
                return
            }
            dialogs.info("Successfully changed AppData directory")
            appDataPath = retVal["newPath"]
        }
    }

    FolderDialog {
        id: browseAppDataPathDialog
        acceptLabel: "Select new appdata directory"
        onAccepted: {
            paneController.changeAppDataDir(selectedFolder)
        }
    }

    ScrollView {
        anchors.fill: parent

        ColumnLayout {
            id: itemsLayout
            width: root.width
            height: root.height

            GridLayout {
                id: grid
                Layout.minimumHeight: 80
                Layout.maximumHeight: 80
                Layout.preferredHeight: 80
                columns: 3
                columnSpacing: 2

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignTop
                    border.width: 3
                    border.color: "green"

                    ColumnLayout {
                        anchors.fill: parent

                        Text {
                            Layout.fillWidth: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: qsTr("Application data path")
                            topPadding: 10
                            leftPadding: 10
                            rightPadding: 10
                            wrapMode: Text.WordWrap
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.minimumHeight: 3
                            Layout.maximumHeight: 3
                            color: "blue"
                        }

                        Text {
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignBottom
                            wrapMode: Text.WordWrap
                            bottomPadding: 10
                            leftPadding: 10
                            rightPadding: 10
                            text: qsTr("This folder contains the logs of the application and temporary files")
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignTop
                    border.color: "green"
                    border.width: 3

                    ColumnLayout {
                        anchors.fill: parent

                        ScrollView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.margins: 10

                            TextArea {
                                id: appDataPathTextArea
                                clip: true
                                text: appDataPath
                                wrapMode: Text.WordWrap
                                readOnly: true
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignTop
                                padding: 5
                            }
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.maximumWidth: grid.width * 0.15
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignTop
                    border.width: 3
                    border.color: "green"

                    ColumnLayout {
                        anchors.fill: parent


                        MyButton {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.maximumHeight: 60
                            Layout.maximumWidth: 100
                            Layout.margins: 10
                            Layout.alignment: Qt.AlignCenter
                            text: qsTr("Browse")
                            textPixelSize: 18
                            Layout.bottomMargin: 0
                            onClicked: {
                                root.spinner = dialogs.spinner("Changing aplication data directory")
                                browseAppDataPathDialog.open()
                            }
                        }

                        MyButton {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.maximumHeight: 60
                            Layout.maximumWidth: 100
                            Layout.margins: 10
                            Layout.alignment: Qt.AlignCenter
                            text: qsTr("Reset")
                            textPixelSize: 18
                            Layout.topMargin: 0
                            highlighted: false
                            flat: false
                            backgroundPressedColor: "#0d9d02"
                            backgroundHoverColor: "#91fb00"
                            backgroundDefaultColor: "#dbca05"
                            onClicked: {
                                dialogs.question(
                                    "The application will close in order to apply the change, you must reopen manually.\n¿Do you want to proceed?",
                                    "Aplication relaunch required",
                                function(ret) {
                                    if (ret["button"] === "YES") {
                                        paneController.resetAppDataDir()
                                        mainController.stopApp()
                                    }
                                })
                            }
                        }
                    }
                }
            }

            GridLayout {
                id: grid2
                Layout.minimumHeight: 50
                Layout.maximumHeight: 50
                Layout.preferredHeight: 50
                columns: 2
                columnSpacing: 2

                Rectangle {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    border.width: 3
                    border.color: "green"

                    Text {
                        height: parent.height
                        width: parent.width
                        text: qsTr("Default project name")
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        padding: 5
                    }
                }

                Rectangle {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    border.width: 3
                    border.color: "green"

                    TextField {
                        anchors.centerIn: parent
                        padding: 5
                        width: parent.width - 10
                        height: parent.height - 10
                        hoverEnabled: true
                        placeholderText: defaultProjectName
                        placeholderTextColor: "grey"
                        text: defaultProjectName
                        maximumLength: 80

                        onTextEdited: {
                            if (text.length < 1) {
                                text = "Project"
                            }
                            paneController.setDefaultProjectName(text)
                        }
                    }
                }
            }


            // Used for compacting the components
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "transparent"
            }
        }
    }

}
