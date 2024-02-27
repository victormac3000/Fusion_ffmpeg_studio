// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: editorPaneRectangle
    width: 1280
    height: 720
    clip: true
    color: "#2e2e2e"

    SplitView {
        id: editorPaneSplitView
        anchors.fill: parent

        handle: Rectangle {
            id: handleDelegate
            implicitWidth: 4
            implicitHeight: parent.height
            color: SplitHandle.pressed ? "#81e889"
                : (SplitHandle.hovered ? Qt.lighter("#c2f4c6", 1.1) : "#c2f4c6")
        }

        VideosGrid {
            id: videosGrid
            objectName: "videosGrid"
            SplitView.fillHeight: true
            SplitView.preferredWidth: 260
        }

        VideoPlayer {
            id: videoPlayerRoot
            objectName: "videoPlayerRoot"
            SplitView.fillHeight: true
            SplitView.fillWidth: true
            color: "#666666"
        }

        QueueGrid {
            id: queueGrid
            SplitView.fillHeight: true
            SplitView.preferredWidth: 250
            // TODO autoresize when extended queueItem
        }

    }
}
