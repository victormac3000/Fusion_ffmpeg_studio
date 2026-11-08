import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import FusionFFmpegStudio

Rectangle {
    id: root
    color: "#141414"

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
                    }

                    Slider {
                        id: slider
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.maximumWidth: 100

                        handle: Rectangle {
                            width: 15
                            height: 15
                            radius: width / 2
                            color: "#f6fbf9"

                            x: parent.leftPadding + parent.visualPosition *
                               (parent.availableWidth - width)
                            y: parent.topPadding + (parent.availableHeight - height) / 2

                            Rectangle {
                                width: parent.width/2
                                height: parent.height/2
                                radius: width / 2
                                anchors.centerIn: parent
                                color: "#00A0FF"
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
                        source: "qrc:/icons/editor/picture-frame-white.png"
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
                            text: "1"
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
                    slider.from = videosFlow.width/4
                    slider.to = videosFlow.width
                    slider.value = videosFlow.width/2 - 10
                }

                function hideVideoGroup(videoGroup) {
                    children.forEach((child) => {
                        if (child instanceof VideoItemNew &&
                            child.video.recordedOnDateString === videoGroup.dateString) {
                            child.visible = !child.visible
                        }
                    })
                }

                function addVideos(videos) {
                    console.log("Received videos:", videos.length)
                    console.log("Slider value:", slider.value)

                    let currentDate
                    let loadError = []

                    videos.forEach((video) => {
                        if (!currentDate || (currentDate !== video.recordedOnDateString)) {
                            currentDate = video.recordedOnDateString
                            addComponent("VideosGroup.qml", {
                                dateString: video.recordedOnDateString,
                                width: videosFlow.width,
                                height: 30,
                                toggleCallback: videosFlow.hideVideoGroup
                            })
                        }
                        if (!addComponent("VideoItemNew.qml", {
                            video: video,
                            width: slider.value,
                            height: slider.value
                        })) {
                            loadError.push(video.idString)
                        }
                    })
                    if (loadError && loadError.length > 0) {
                        console.error("Could not load videos:", loadError)
                        dialogs.warning(qsTr("Some videos could not be loaded"), qsTr("Video loading error"))
                    }

                    console.log("Flow children:", videosFlow.children.length)
                    console.log("Flow dimensions:", videosFlow.width, videosFlow.height)

                    for (const child of videosFlow.children) {
                        console.log(
                            "Child size:", child.width, child.height,
                            "Implicit size:", child.implicitWidth, child.implicitHeight,
                            "Position:", child.x, child.y
                        )
                    }
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