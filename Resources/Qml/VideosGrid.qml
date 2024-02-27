import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Rectangle {
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
                objectName: "videosGridLayout"
                columns: {
                    return videosRectangle.width / 125
                }

                signal activeVideoChanged(position: variant)
                property VideoItem selectedVideo: null

                onSelectedVideoChanged: {
                    activeVideoChanged(selectedVideo);
                }

                Component.onCompleted: {
                    /*
                    for (var i=0; i<100; i++) {
                        addVideo(-1, "", "", "")
                    }
                    */
                }

                function addVideo(id, idstring, imagePath, recorded) {
                    var videoComponent = Qt.createComponent("VideoItem.qml")
                    var videoElement = videoComponent.createObject(videosGridLayout)
                    videoElement.setData(id, idstring, imagePath, recorded)
                    if (selectedVideo == null) {
                        videoItemClicked(videoElement)
                    }
                    return videoElement
                }

                function videoItemClicked(videoItem) {
                    if (selectedVideo != null) {
                        selectedVideo.setSelected(false)
                    }
                    videoItem.setSelected(true);
                    selectedVideo = videoItem
                }

                function getSelectedVideoId() {
                    return selectedVideo.id
                }
            }

            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AlwaysOn
            }
        }

        RenderOptions {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 130
        }

        Rectangle {
            id: videosGridBottomOptions
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 30
            color: "#353637"

            RowLayout {
                anchors.fill: parent
                Button {
                    objectName: "renderPreviewButton"
                    text: "Render preview"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
                Button {
                    objectName: "renderButton"
                    text: "Render"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
            }
        }
    }


}
