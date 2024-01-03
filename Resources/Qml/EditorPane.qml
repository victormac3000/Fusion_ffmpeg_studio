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
            SplitView.minimumWidth: 260
            implicitWidth: parent.width*0.2
            color: "#353637"

            ScrollView {
                id: videosScrollView
                objectName: "videosScrollView"
                clip: true

                height: parent.height
                width: parent.width
                ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOn

                Component.onCompleted: {
                    for (var i=0; i<10; i++)
                        videosGridView.addVideo()
                }

                GridView {
                    property VideoItem selectedVideo
                    property ListModel videosModel: ListModel {}

                    id: videosGridView
                    objectName: "videosGridView"
                    width: videosScrollView.width
                    height: videosScrollView.height
                    cellWidth: 125
                    cellHeight: 125
                    model: videosModel

                    delegate: VideoItem {
                        width: videosGridView.cellWidth
                        height: videosGridView.cellHeight
                    }

                    function videoItemClicked(videoItemMouseArea) {
                        var videoItem = videoItemMouseArea.parent
                        var videoItemModel = videoItem.getModel()
                        if (selectedVideo != null) {
                            selectedVideo.selected = false
                        }
                        videoItemModel.selected = true
                        selectedVideo = videoItem
                    }

                    function addVideo() {
                        var video = Qt.createComponent("VideoItem.qml")
                        var videoItem = video.createObject()
                        var videoItemModel = videoItem.getModel()
                        if (selectedVideo == null) {
                            videoItemModel.selected = true
                            selectedVideo = videoItem
                        }
                        videosModel.append(videoItem)
                        return videoItemModel;
                    }

                    function getNumVideos() {
                        return count;
                    }
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
