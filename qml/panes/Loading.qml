import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import FusionFFmpegStudio

Rectangle {
    id: root
    color: "lightblue"

    required property string operation
    property string projectName
    property string projectPath
    property string frontPath
    property string backPath
    property string dcimPath
    property bool dcimCopy

    LoadingController {
        id: controller
        appController: mainController
    }

    Dialogs {
        id: dialogs
    }

    Component.onCompleted: {
        var args = {
            operation: operation,
            projectName: projectName,
            projectPath: projectPath,
            frontPath: frontPath,
            backPath: backPath,
            dcimPath: dcimPath,
            dcimCopy: dcimCopy
        }
        controller.startLoading(
            args,
            () => {
                console.log("Project loaded ok")
            },
            (error) => {
                dialogs.warning(error, "Loading error", () => {
                    mainController.back()
                })
            }
        )
    }

    ColumnLayout {
        anchors.fill: parent

        Image {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: parent.height*0.8
            source: "qrc:/images/Snow.jpg"
        }

        GridLayout {
            objectName: "progressBarGrid"
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 10
            columns: 3
            rows: 2
            property bool generalMessage: true
            property bool specificMessage: true

            Text {
                objectName: "mainMessageTopText"
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.columnSpan: 3
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Text {
                Layout.fillHeight: true
                Layout.fillWidth: true
                objectName: "mainMessageLeftText"
                text: "0%"
                horizontalAlignment: Text.AlignRight
            }

            NiceProgressBar {
                Layout.fillHeight: true
                Layout.fillWidth: true
                objectName: "mainMessageBar"
                insideText: ""
                from: 0.0
                to: 100.0
            }

            Text {
                Layout.fillHeight: true
                Layout.fillWidth: true
                objectName: "mainMessageRightText"
                text: "100%"
                horizontalAlignment: Text.AlignLeft
            }

            Text {
                visible: parent.generalMessage
                objectName: "generalMessageTopText"
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.columnSpan: 3
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Text {
                visible: parent.generalMessage
                Layout.fillHeight: true
                Layout.fillWidth: true
                objectName: "generalMessageLeftText"
                text: "0%"
                horizontalAlignment: Text.AlignRight
            }

            NiceProgressBar {
                visible: parent.generalMessage
                Layout.fillHeight: true
                Layout.fillWidth: true
                objectName: "generalMessageBar"
                insideText: ""
                from: 0.0
                to: 100.0
            }

            Text {
                visible: parent.generalMessage
                Layout.fillHeight: true
                Layout.fillWidth: true
                objectName: "generalMessageRightText"
                text: "100%"
                horizontalAlignment: Text.AlignLeft
            }

            Text {
                visible: parent.specificMessage
                objectName: "specificMessageTopText"
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.columnSpan: 3
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Text {
                visible: parent.specificMessage
                Layout.fillHeight: true
                Layout.fillWidth: true
                objectName: "specificMessageLeftText"
                text: "0%"
                horizontalAlignment: Text.AlignRight
            }

            NiceProgressBar {
                visible: parent.specificMessage
                Layout.fillHeight: true
                Layout.fillWidth: true
                objectName: "specificMessageBar"
                insideText: ""
                from: 0.0
                to: 100.0
            }

            Text {
                visible: parent.specificMessage
                Layout.fillHeight: true
                Layout.fillWidth: true
                objectName: "specificMessageRightText"
                text: "100%"
                horizontalAlignment: Text.AlignLeft
            }
        }

    }

}
