// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 6.5
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    width: 1280
    height: 720

    SplitView {
        id: splitView
        anchors.fill: parent

        handle: Rectangle {
            id: handleDelegate
            implicitWidth: 4
            implicitHeight: 4
            color: SplitHandle.pressed ? "#81e889"
                : (SplitHandle.hovered ? Qt.lighter("#c2f4c6", 1.1) : "#c2f4c6")

            containmentMask: Item {
                x: (handleDelegate.width - width) / 2
                width: 64
                height: splitView.height
            }
        }

        Rectangle {
            id: videosRectangle
            SplitView.fillWidth: true
            SplitView.minimumWidth: 200
            implicitWidth: parent.width*0.2
            color: "#353637"

            ScrollView {
                id: videosScrollView
                objectName: "videosScrollView"
                width: parent.width
                height: parent.height
                contentWidth: videosGridLayout.width
                ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOn

                Component.onCompleted: {
                    for (var i=0; i<10; i++)
                        videosGridLayout.addVideoItem()
                }

                GridLayout {
                    property VideoItem selectedVideo

                    id: videosGridLayout
                    objectName: "videosGridLayout"
                    width: videosRectangle.width
                    height: videosRectangle.height
                    columnSpacing: 5
                    rows: 3
                    columns: 2

                    function videoItemClicked(videoItem) {
                        if (selectedVideo != undefined) {
                            selectedVideo.selected = false;
                        }
                        videoItem.selected = true
                        selectedVideo = videoItem
                    }

                    function addVideoItem() {
                        var newVideoItem = Qt.createQmlObject('import QtQuick 2.15; VideoItem {}', videosGridLayout)
                        newVideoItem.Layout.alignment = Qt.AlignTop
                        newVideoItem.Layout.fillWidth = true
                        Qt.createQmlObject('import QtQuick 2.15; MouseArea {}', newVideoItem)
                        return newVideoItem;
                    }

                }
            }

        }

        Rectangle {
            SplitView.fillWidth: true
            SplitView.minimumWidth: 300
            implicitWidth: parent.width*0.6
            color: "#666"

            VideoPlayerUx {
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
