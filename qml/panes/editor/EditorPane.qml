// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtQuick.Layouts

import FusionFFmpegStudio

Rectangle {
    id: root
    color: "#2e2e2e"

    Component.onCompleted: {
        //parent.windowMaximize()
    }

    EditorController {
        id: controller
        appController: mainController
    }

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        spacing: 0

        EditorPaneHeader {
            id: header
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            Layout.maximumHeight: 60
        }
    }


}
