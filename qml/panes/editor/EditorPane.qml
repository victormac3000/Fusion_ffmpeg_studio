// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import FusionFFmpegStudio

Rectangle {
    id: root
    color: "#2e2e2e"

    Component.onCompleted: {
        parent.windowMaximize()
    }

    EditorController {
        id: controller
        appController: mainController
    }

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
            SplitView.fillHeight: true
            SplitView.preferredWidth: 260
            SplitView.minimumWidth: 200
            controller: controller
            videoPlayer: videoPlayerRoot
        }

        VideoPlayer {
            id: videoPlayerRoot
            SplitView.fillHeight: true
            SplitView.fillWidth: true
            SplitView.minimumWidth: 250
            color: "#666666"
            controller: controller
        }

        QueueGrid {
            // TODO autoresize when extended queueItem
            id: queueGrid
            SplitView.fillHeight: true
            SplitView.preferredWidth: 250
            SplitView.minimumWidth: 100
            controller: controller
        }
    }
}
