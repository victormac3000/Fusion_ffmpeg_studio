import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import FusionFFmpegStudio

Rectangle {
    required property var controller
    required property var videoPlayer

    id: videosRectangle
    color: "#353637"

    ColumnLayout {
        anchors.fill: parent

        Flickable {
            id: videosGridFlickable
            Layout.fillHeight: true
            Layout.fillWidth: true
            flickableDirection: Flickable.VerticalFlick
            boundsBehavior: Flickable.DragOverBounds
            clip: true
            contentHeight: videosGridLayout.height


            GridLayout {
                id: videosGridLayout
                columns: {
                    return videosRectangle.width / 125
                }

                property var selectedVideo

                Component.onCompleted: {
                    controller.getVideos()
                }

                Connections {
                    target: controller

                    function onVideosReady(videos) {
                        videos.forEach((video) => {
                            videosGridLayout.addVideo(video)
                        })
                        if (videos.length > 0) {
                            console.log("Sending video")
                            videoPlayer.setVideo(videos[0])
                        }
                    }
                }

                function onVideoSelected(video) {
                    selectedVideo = video
                    video.selected = true
                }

                function addVideo(video) {
                    var videoComponent = Qt.createComponent("VideoItem.qml")

                    if (videoComponent.status === Component.Ready) {
                        var args = {
                            video: video,
                            clickHandler: onVideoSelected
                        }
                        var videoElement = videoComponent.createObject(videosGridLayout, args)
                        if (videoElement === null) {
                            console.log("Could not create object VideoItem")
                            dialogs.warning("Could not load some videos", "UI error")
                        }
                    } else {
                        console.error("Could not create component VideoItem: " + videoComponent.errorString())
                        dialogs.warning("Could not load some videos", "UI error")
                    }
                }
            }

            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AlwaysOn
            }
        }
    }


}
