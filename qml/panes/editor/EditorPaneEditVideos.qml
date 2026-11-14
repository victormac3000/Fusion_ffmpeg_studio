import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import FusionFFmpegStudio

Rectangle {
    id: root
    color: "#141414"

    property var selectedVideo

    EditorController {
        id: controller
        appController: mainController
    }

    Dialogs {
        id: dialogs
    }

    Component.onCompleted: {
        controller.getVideos()
    }

    ColumnLayout {
        id: editVideosLayout
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 60
            color: "#353535"

            StackLayout {
                id: stackLayout
                anchors.fill: parent
                anchors.margins: 15
                currentIndex: 0

                onCurrentIndexChanged: {
                    parent.color = (currentIndex === 1) ? "#00A1E0" : "#353535"
                }

                RowLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    spacing: 10

                    Text {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        font.bold: true
                        color: "#F6FBF9"
                        text: qsTr("CAMERA MEDIA")
                        verticalAlignment: Text.AlignVCenter
                    }

                    Item {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }

                    Image {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.maximumHeight: parent.height * 0.7
                        Layout.maximumWidth: parent.height * 0.7
                        Layout.rightMargin: 10
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/icons/editor/picture-frame-white.png"

                        MouseArea {
                            anchors.fill: parent

                            onClicked: {
                                slider.value = slider.from
                            }
                        }
                    }

                    MySlider {
                        id: slider
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.maximumWidth: 100
                    }

                    Image {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.maximumHeight: parent.height * 0.9
                        Layout.maximumWidth: parent.height * 0.9
                        Layout.leftMargin: 10
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/icons/editor/picture-frame-white.png"

                        MouseArea {
                            anchors.fill: parent

                            onClicked: {
                                slider.value = slider.to
                            }
                        }
                    }

                    Image {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.maximumHeight: parent.height * 0.9
                        Layout.maximumWidth: parent.height * 0.9
                        Layout.leftMargin: 10
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/icons/editor/checkbox-white.png"

                        MouseArea {
                            anchors.fill: parent

                            onClicked: {
                                videosFlow.selectedVideosCount = 1
                                stackLayout.currentIndex = 1
                            }
                        }
                    }
                }

                RowLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    spacing: 5

                    Text {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.maximumWidth: 30

                        bottomPadding: 3

                        text: "×"
                        font.pixelSize: 1000
                        fontSizeMode: Text.Fit
                        minimumPixelSize: 1

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter

                        color: "#F6FBF9"

                        MouseArea {
                            anchors.fill: parent

                            onClicked: {
                                stackLayout.currentIndex = 0
                            }
                        }
                    }

                    ToolSeparator {
                        id: toolSeparator
                        Layout.fillHeight: true
                    }

                    RowLayout {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        spacing: 3

                        Text {
                            Layout.fillHeight: true
                            verticalAlignment: Text.AlignVCenter
                            color: "#F6FBF9"
                            text: videosFlow.selectedVideosCount
                            font.bold: true
                        }

                        Text {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            verticalAlignment: Text.AlignVCenter
                            color: "#F6FBF9"
                            text: qsTr("SELECTED SHOT")
                        }
                    }

                    Item {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }

                    Image {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.maximumHeight: parent.height * 0.7
                        Layout.maximumWidth: parent.height * 0.7
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/icons/editor/trash-bin-white.png"
                        visible: false // TODO: Implement video deletion
                    }

                    Image {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.maximumHeight: parent.height * 0.9
                        Layout.maximumWidth: parent.height * 0.9
                        Layout.leftMargin: 10
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/icons/editor/checkbox-white.png"

                        MouseArea {
                            anchors.fill: parent

                            onClicked: {
                                if (videosFlow.selectedVideosCount < 1) {
                                    videosFlow.selectLastVideo()
                                }
                                stackLayout.currentIndex = 0
                            }
                        }
                    }
                }
            }
        }

        Flickable {
            id: videoFlickable

            clip: true

            Layout.fillHeight: true
            Layout.fillWidth: true

            contentWidth: width
            contentHeight: videosFlow.height + 20

            boundsBehavior: Flickable.StopAtBounds

            Flow {
                id: videosFlow

                property var videoGroups: []
                property int selectedVideosCount: 1

                Connections {
                    target: controller

                    function onVideosReady(videos) {
                        videosFlow.addVideos(videos)
                    }
                }

                anchors {
                    left: parent.left
                    right: parent.right
                    leftMargin: 30
                    rightMargin: 30
                    top: parent.top
                    topMargin: 10
                }
                height: implicitHeight
                spacing: 10

                Component.onCompleted: {
                    var from = videosFlow.width/4
                    // If from is less than minimum video width
                    if (from < 200) {
                        from = 200
                    }
                    slider.from = from
                    slider.to = videosFlow.width
                    slider.value = videosFlow.width/2 - 10
                }

                onWidthChanged: {
                    slider.from = videosFlow.width/4
                    slider.to = videosFlow.width
                }

                function selectLastVideo() {
                    var lastVideo = videoGroups[videoGroups.length-1].video
                    lastVideo.selected = true
                    root.selectedVideo = lastVideo
                }

                function hideVideoGroup(videoGroup) {
                    children.forEach((child) => {
                        if (child instanceof VideoItemNew &&
                            child.video.recordedOn.getDate() === videoGroup.date.getDate()) {
                            child.visible = !child.visible
                        }
                    })
                }

                function canSelectVideo(video) {
                    if (stackLayout.currentIndex === 0) {
                        videoGroups.forEach((videoGroup) => {
                            if (videoGroup.video.selected) {
                                videoGroup.video.selected = false
                            }
                        })
                        root.selectedVideo = video
                    }
                    selectedVideosCount++

                    return true
                }

                function canDeselectVideo() {
                    if (selectedVideosCount-1 >= 0) {
                        selectedVideosCount--;
                    }
                    return true
                }

                function addVideos(videos) {
                    let currentDate
                    let loadErrors = []

                    videos.forEach((video) => {
                        if (!currentDate || (currentDate.getDate() !== video.recordedOn.getDate())) {
                            currentDate = video.recordedOn
                            var videoGroup = addComponent("VideosGroup.qml", {
                                date: video.recordedOn,
                                width: Qt.binding(() => videosFlow.width),
                                height: 30,
                                toggleCallback: videosFlow.hideVideoGroup
                            })
                        }

                        var videoComponent = addComponent("VideoItemNew.qml", {
                            video: video,
                            width: Qt.binding(() => slider.value),
                            height: Qt.binding(() => slider.value),
                            canSelect: canSelectVideo,
                            canDeselect: canDeselectVideo
                        });

                        videoGroups.push({
                            video: videoComponent,
                            group: videoGroup
                        })

                        if (!videoGroup || !videoComponent) {
                            if (!videoGroup) {
                                console.error("Could not create video group for video ", video.idString)
                            }
                            if (!videoComponent) {
                                console.error("Could not create video component for video ", video.idString)
                            }
                            loadErrors.push(video.idString)
                        }
                    })

                    if (loadErrors && loadErrors.length > 0) {
                        dialogs.warning(qsTr("Some videos could not be loaded"), qsTr("Video loading error"))
                    }

                    selectLastVideo()
                }

                function addComponent(componentUrl, properties = {}) {
                    const component = Qt.createComponent(componentUrl)

                    if (component.status === Component.Ready) {
                        return component.createObject(videosFlow, properties)
                    }

                    if (component.status === Component.Error) {
                        console.error("Could not create component " + componentUrl + ": " + component.errorString())
                        return null
                    }

                    return null
                }

                move: Transition {
                    NumberAnimation {
                        properties: "x,y"
                        duration: 150
                    }
                }
            }

            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
            }

            ScrollBar.horizontal: ScrollBar {
                policy: ScrollBar.AsNeeded
            }
        }
    }
}