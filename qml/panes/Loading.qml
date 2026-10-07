import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import FusionFFmpegStudio

Rectangle {
    id: root
    color: "lightblue"

    required property int operation
    property string projectName
    property string projectPath
    property string frontPath
    property string backPath
    property string dcimPath
    property bool dcimCopy

    Connections {
        target: controller

        function onLoadProjectDone(data) {
            if (data.numVideos < 1) {
                dialogs.error("Could not detect any valid GoPro Fusion source video files", "No videos", () => {
                    mainController.back()
                })
            }

            if (data.badVideos.length > 0) {
                var msg = "Some videos could not be detected and were skipped\n"
                data.badVideos.forEach((videoID) => {
                    msg += videoID + ","
                })
                if (msg.charAt(msg.length-1) == ",") {
                    msg = msg.slice(0, -1)
                }
                dialogs.warning(msg, "Invalid videos", () => {
                    console.log("LOAD EDITOR")
                })
            }
        }

        function onLoadProjectUpdate(data) {
            mainLoadingBar.insideText = data.operation
            mainLoadingBar.value = data.stepNumber/(data.stepCount-1)
            mainLeftLabel.text = data.stepNumber + 1
            mainRightLabel.text = data.stepCount
            auxLoadingBar.hoverText = ""

            if (data.stepID === constants.copyDCIMFolder) {
                var copy = data.copy;
                var currentFile = data.copy.currentFile;

                var filePercent = currentFile.bytesDone/currentFile.bytesCount
                auxLoadingBar.insideText = "Copying " + currentFile.name;
                if (currentFile.speed > 0) {
                    auxLoadingBar.hoverText = currentFile.speed.toFixed(1) + " MB/s"
                }
                auxLoadingBar.value = filePercent
                auxLeftLabel.text = (filePercent*100).toFixed(0) + "%"

                var totalPercent = (copy.fileNumber-1+filePercent)/copy.fileCount
                secLeftLabel.text = (totalPercent*100).toFixed(0) + "%"
                secLoadingBar.insideText = (totalPercent*100).toFixed(0) + "% ("
                        + copy.fileNumber + " of " + copy.fileCount + ")"
                secLoadingBar.value = totalPercent

                secProgressRow.visible = true
                auxProgressRow.visible = true
            }

            if (data.stepID === constants.indexVideos) {
                var index = data.index

                var segmentPercent = index.doneSegments/index.totalSegments
                auxLoadingBar.insideText = "Indexing segment " + index.doneSegments + " of " + index.totalSegments
                auxLoadingBar.value = segmentPercent
                auxLeftLabel.text = (segmentPercent*100).toFixed(0) + "%"

                var videoPercent = (index.doneVideos-1+segmentPercent)/index.totalVideos
                secLoadingBar.insideText = "Indexing video " + index.doneVideos + " of " + index.totalVideos
                secLoadingBar.value = videoPercent
                secLeftLabel.text = (videoPercent*100).toFixed(0) + "%"

                secProgressRow.visible = true
                auxProgressRow.visible = true
            }
        }

        function onLoadProjectError(data) {
            dialogs.warning(data.message, data.title, () => {
                mainController.back()
            })
        }
    }

    Constants {
        id: constants
        visible: false
    }

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

        ColumnLayout {
            id: progressLayout
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 10

            property real leftRightWidth: 45

            RowLayout {
                id: mainProgressRow
                Layout.fillHeight: true
                Layout.fillWidth: true

                Text {
                    id: mainLeftLabel
                    Layout.fillHeight: true
                    Layout.preferredWidth: progressLayout.leftRightWidth
                    Layout.minimumWidth: progressLayout.leftRightWidth
                    Layout.maximumWidth: progressLayout.leftRightWidth
                    text: "1"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }

                NiceProgressBar {
                    id: mainLoadingBar
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                Text {
                    id: mainRightLabel
                    Layout.fillHeight: true
                    Layout.preferredWidth: progressLayout.leftRightWidth
                    Layout.minimumWidth: progressLayout.leftRightWidth
                    Layout.maximumWidth: progressLayout.leftRightWidth
                    text: "1"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }

            RowLayout {
                id: secProgressRow
                visible: false
                Layout.fillHeight: true
                Layout.fillWidth: true

                Text {
                    id: secLeftLabel
                    Layout.fillHeight: true
                    Layout.preferredWidth: progressLayout.leftRightWidth
                    Layout.minimumWidth: progressLayout.leftRightWidth
                    Layout.maximumWidth: progressLayout.leftRightWidth
                    text: "0%"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }

                NiceProgressBar {
                    id: secLoadingBar
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                Text {
                    id: secRightLabel
                    Layout.fillHeight: true
                    Layout.preferredWidth: progressLayout.leftRightWidth
                    Layout.minimumWidth: progressLayout.leftRightWidth
                    Layout.maximumWidth: progressLayout.leftRightWidth
                    text: "100%"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }

            RowLayout {
                id: auxProgressRow
                visible: false
                Layout.fillHeight: true
                Layout.fillWidth: true

                Text {
                    id: auxLeftLabel
                    Layout.fillHeight: true
                    Layout.preferredWidth: progressLayout.leftRightWidth
                    Layout.minimumWidth: progressLayout.leftRightWidth
                    Layout.maximumWidth: progressLayout.leftRightWidth
                    text: "0%"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }

                NiceProgressBar {
                    id: auxLoadingBar
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                Text {
                    id: auxRightLabel
                    Layout.fillHeight: true
                    Layout.preferredWidth: progressLayout.leftRightWidth
                    Layout.minimumWidth: progressLayout.leftRightWidth
                    Layout.maximumWidth: progressLayout.leftRightWidth
                    text: "100%"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
}
