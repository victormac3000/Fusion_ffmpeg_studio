// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 6.5
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: editorPaneRectangle
    width: 1280
    height: 720

    SplitView {
        id: splitView
        anchors.fill: parent

        handle: Rectangle {
            id: handleDelegate
            implicitWidth: 4
            implicitHeight: splitView.height
            color: SplitHandle.pressed ? "#81e889"
                : (SplitHandle.hovered ? Qt.lighter("#c2f4c6", 1.1) : "#c2f4c6")
        }

        Rectangle {
            id: videosRectangle
            SplitView.fillWidth: true
            SplitView.minimumWidth: 270
            implicitWidth: parent.width*0.2
            color: "#353637"

            Flickable {
                anchors.fill: parent
                flickableDirection: Flickable.VerticalFlick
                boundsBehavior: Flickable.DragOverBounds
                clip: true
                contentHeight: videosGridLayout.height

                GridLayout {
                    id: videosGridLayout
                    objectName: "videosGridLayout"
                    columns: 2
                    property VideoItem selectedVideo: null

                    Component.onCompleted: {
                        /*
                        for (var i=0; i<100; i++) {
                            var videoComponent = Qt.createComponent("VideoItem.qml")
                            var videoElement = videoComponent.createObject(videosGridLayout)
                        }
                        */
                    }

                    function addVideo(idstring, imagePath, recorded) {
                        var videoComponent = Qt.createComponent("VideoItem.qml")
                        var videoElement = videoComponent.createObject(videosGridLayout)
                        if (selectedVideo == null) {
                            videoItemClicked(videoElement)
                        }
                    }

                    function videoItemClicked(videoItem) {
                        if (selectedVideo != null) {
                            selectedVideo.setSelected(false)
                        }
                        videoItem.setSelected(true);
                        selectedVideo = videoItem
                    }
                }

                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AlwaysOn
                }
            }
        }

        Rectangle {
            SplitView.fillWidth: true
            SplitView.minimumWidth: 300
            implicitWidth: parent.width*0.6
            color: "#666666"

            VideoPlayerMain {
                id: videoPlayerRoot
                objectName: "videoPlayerRoot"
                width: parent.width
                height: parent.height
            }
        }

        Rectangle {
            id: queueRectangle
            objectName: "queueRectangle"
            SplitView.fillWidth: true
            SplitView.minimumWidth: 200
            implicitWidth: parent.width*0.2
            color: "#666"

            ScrollView {
                id: queueScrollView
                objectName: "queueScrollView"
                width: parent.width
                height: parent.height
                contentWidth: queueLayout.width
                ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOn

                Component.onCompleted: {
                    /*
                    for (var i=0; i<10; i++)
                        queueLayout.addQueueItem()*/
                }

                ColumnLayout {
                    id: queueLayout
                    objectName: "queueLayout"
                    width: queueRectangle.width
                    height: queueRectangle.height

                    function addQueueItem() {
                        var newQueueItem = Qt.createQmlObject('import QtQuick 2.15; QueueItem {}', queueLayout);
                        newQueueItem.Layout.alignment = Qt.AlignTop
                    }
                }

            }


        }
    }
}
