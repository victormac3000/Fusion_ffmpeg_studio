// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 6.2
import Fusion_Ffmpeg_Studio_Qml
import QtQuick.Controls 6.2
import QtQuick.Layouts

Window {
    width: 1280
    height: 720

    visible: true
    title: "Fusion_Ffmpeg_Studio_Qml"

    RowLayout {
        anchors.fill: parent

        EditorPane {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }



}

