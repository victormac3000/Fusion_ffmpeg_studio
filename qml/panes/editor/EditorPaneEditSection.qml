import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import FusionFFmpegStudio

Rectangle {
    id: root
    color: "#141414"

    property var selectedVideo

    onSelectedVideoChanged: {
        if (!root.selectedVideo) return

        console.log("Selected video changed")
        root.selectedVideo.onDataChanged.connect((videoData) => {
            videoInfoLayout.name = videoData.name
        })

        // Load a video
        /*
        var v = root.selectedVideo.video
        videoPlayer.loadVideo(v.frontURL, v.backURL, v.savedSettings || {
            // fallback defaults if the video was never edited
            yaw:         0,
            pitch:       0,
            roll:        0,
            cuttingMode: false,
            selStart:    0,
            selEnd:      1,
            playerMode:  false
        })
        */
    }

    ColumnLayout {
        id: editPlayerLayout
        anchors.fill: parent
        spacing: 0

        // ---------- Header ----------
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            Layout.maximumHeight: 60
            color: "#222222"

            ColumnLayout {
                id: videoInfoLayout
                anchors.fill: parent
                anchors.leftMargin: 20
                anchors.rightMargin: 20
                anchors.topMargin: 5
                anchors.bottomMargin: 5
                spacing: 0

                property var name: root.selectedVideo ? root.selectedVideo.video.name       : ""
                property var recordedOn: root.selectedVideo ? root.selectedVideo.video.recordedOn : null

                Item { Layout.fillHeight: true; Layout.fillWidth: true }

                MyText {
                    Layout.fillWidth: true
                    font.bold: true
                    text: videoInfoLayout.name
                    elide: Text.ElideRight
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 0

                    MyText {
                        font.pointSize: 12
                        text: videoInfoLayout.recordedOn
                              ? Qt.formatDate(videoInfoLayout.recordedOn, "dd MMM, yyyy").toUpperCase() + " - "
                              : ""
                    }
                    MyText {
                        font.bold: true
                        font.pointSize: 12
                        text: videoInfoLayout.recordedOn
                              ? Qt.formatTime(videoInfoLayout.recordedOn, "hh:mm")
                              : ""
                    }
                }

                Item { Layout.fillHeight: true; Layout.fillWidth: true }
            }
        }

        StackLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            currentIndex: 0

            Rectangle {
                color: "blue"
            }

            EditorPaneEditPlayer {
                id: videoPlayer
                Layout.fillHeight: true
                Layout.fillWidth: true

                onSettingsChanged: (settings) => {
                    if (parent.currentIndex !== 1) return
                    console.log("New settings:", JSON.stringify(settings))
                    root.selectedVideo.video.savedSettings = settings
                }

                onProcessRequested: (settings) => {
                    console.log("Render clicked")
                }
            }
        }
    }
}