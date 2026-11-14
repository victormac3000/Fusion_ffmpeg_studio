import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtWebEngine
import QtWebChannel

import FusionFFmpegStudio

Rectangle {
    id: root
    color: "#141414"

    property var selectedVideo

    onSelectedVideoChanged: {
        root.selectedVideo.onDataChanged.connect((videoData) => {
            videoInfoLayout.name = videoData.name
        })
        webView.loadVideos(
            root.selectedVideo.video.frontURL,
            root.selectedVideo.video.backURL
        )
    }

    ColumnLayout {
        id: editPlayerLayout
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 60
            color: "#222222"

            ColumnLayout {
                id: videoInfoLayout
                anchors.fill: parent
                anchors.leftMargin: 20
                anchors.rightMargin: 20
                anchors.topMargin: 5
                anchors.bottomMargin: 5
                spacing: 0

                property var name: root.selectedVideo ? root.selectedVideo.video.name   : ""
                property var recordedOn: root.selectedVideo ? root.selectedVideo.video.recordedOn : null

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                MyText {
                    Layout.fillWidth: true
                    font.bold: true
                    text: videoInfoLayout.name
                    elide: Text.ElideRight
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 0

                    MyText {
                        text: {
                            if (videoInfoLayout.recordedOn) {
                                return Qt.formatDate(videoInfoLayout.recordedOn, "dd MMM, yyyy").toUpperCase() + " - "
                            }
                            return ""
                        }
                        font.pointSize: 12
                    }

                    MyText {
                        font.bold: true
                        font.pointSize: 12
                        text: {
                            if (videoInfoLayout.recordedOn) {
                                return Qt.formatTime(videoInfoLayout.recordedOn, "hh:mm")
                            }
                            return ""
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }


            }
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "grey"

            StackLayout {
                anchors.fill: parent
                currentIndex: 1

                Rectangle {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    color: "grey"

                    Spinner {
                        anchors.centerIn: parent
                        spinnerText: qsTr("Generating preview\n10%")
                        spinnerColor: "#F6FBF9"
                        spinnerSize: 40
                        lineWidth: 4
                    }
                }

                WebEngineView {
                    id: webView
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    url: "qrc:/web/videoplayer/index.html"
                    webChannel: WebChannel {
                        registeredObjects: [webViewBridge]
                    }

                    property var  pendingVideos: null
                    property bool viewerInitialized: false

                    onJavaScriptConsoleMessage: function(level, message, lineNumber, sourceID) {
                        console.log("[JS]", sourceID, ":", lineNumber, "->", message)
                    }

                    function loadVideos(frontURL, backURL) {
                        pendingVideos = [frontURL, backURL]
                        if (viewerInitialized) _doLoadVideos()
                        else console.log("loadVideos queued – waiting for viewer ready")
                    }

                    function _doLoadVideos() {
                        if (!pendingVideos) return
                        const frontURL = pendingVideos[0]
                        const backURL  = pendingVideos[1]
                        pendingVideos  = null

                        const js = `
                            (window.fusionViewer &&
                             typeof window.fusionViewer.loadVideos === 'function')
                                ? (window.fusionViewer.loadVideos(
                                        ${JSON.stringify(frontURL)},
                                        ${JSON.stringify(backURL)}), 'ok')
                                : 'viewer-not-ready';
                        `
                        webView.runJavaScript(js, function(result) {
                            console.log("loadVideos() returned:", result)
                            if (result === 'viewer-not-ready') {
                                pendingVideos = [frontURL, backURL]   // retry
                                retryTimer.restart()
                            }
                        })
                    }

                    Timer {
                        id: retryTimer
                        interval: 200
                        onTriggered: webView._doLoadVideos()
                    }

                    QtObject {
                        id: webViewBridge
                        WebChannel.id: "fusionViewerBridge"

                        function ready(data) {
                            console.log("Viewer ready:", JSON.stringify(data))
                            if (data && data.initialized === true) {
                                webView.viewerInitialized = true
                                webView._doLoadVideos()
                            }
                        }

                        function playingChanged(playing) {
                            console.log("Playing:", playing)
                        }
                        function cameraChanged(data) {
                            //console.log("Camera:", JSON.stringify(data))
                        }
                        function profileChanged(data) {
                            console.log("Active profile:", data.name)
                        }
                    }
                }
            }
        }

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 25
            Layout.margins: 10

            MyText {
                text: "00:00"
            }

            MySlider {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            MyText {
                text: "09:00"
            }
        }

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 20
            Layout.margins: 10
            spacing: 20

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            RowLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                spacing: 2

                property color color: "#F6FBF9"

                Triangle {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.maximumWidth: 25

                    color: parent.color
                    direction: Triangle.Left
                }

                Rectangle {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.maximumWidth: 5
                    color: parent.color
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: {
                        parent.color = "#00A0FF"
                    }

                    onExited: {
                        parent.color = "#F6FBF9"
                    }
                }
            }

            RowLayout {
                id: pauseIcon
                visible: false
                Layout.fillHeight: true
                Layout.fillWidth: true
                spacing: 2

                property color color: "#F6FBF9"

                Rectangle {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.maximumWidth: 5
                    color: parent.color
                }

                Rectangle {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.maximumWidth: 5
                    color: parent.color
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: {
                        parent.color = "#00A0FF"
                    }

                    onExited: {
                        parent.color = "#F6FBF9"
                    }

                    onClicked: {
                        pauseIcon.visible = false
                        playIcon.visible = true
                    }
                }
            }

            Triangle {
                id: playIcon
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.maximumWidth: 25

                color: "#F6FBF9"
                direction: Triangle.Right

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: {
                        parent.color = "#00A0FF"
                    }

                    onExited: {
                        parent.color = "#F6FBF9"
                    }

                    onClicked: {
                        playIcon.visible = false
                        pauseIcon.visible = true
                    }
                }
            }

            RowLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                spacing: 2

                property color color: "#F6FBF9"

                Rectangle {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.maximumWidth: 5
                    color: parent.color
                }


                Triangle {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.maximumWidth: 25

                    color: parent.color
                    direction: Triangle.Right
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: {
                        parent.color = "#00A0FF"
                    }

                    onExited: {
                        parent.color = "#F6FBF9"
                    }
                }
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 50

            color: "#222222"

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10

                MyText {
                    text: "360"
                    color: playerMode.checked ? "#879899" : "#00A1E1"
                }

                MySwitch {
                    id: playerMode
                    Layout.fillHeight: true

                    knobMargin: 1
                    knobColorOff: "#00A1E1"
                    knobColorOn: "#00A1E1"
                    trackColorOff: "transparent"
                    trackColorOn: "transparent"
                    trackBorderWidth: 3
                    trackBorderColorOn:  "#F6FBF9"
                    trackBorderColorOff: "#F6FBF9"
                }

                MyText {
                    text: "OVERCAPTURE"
                    color: playerMode.checked ? "#00A1E1" : "#879899"
                }

                Item {
                    Layout.fillWidth: true
                }
            }
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 50
            color: "#1D1D1D"

            RowLayout {
                anchors.fill: parent
                anchors.margins: 20

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                Image {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.maximumWidth: resetButtonText.height
                    Layout.maximumHeight: resetButtonText.height
                    Layout.alignment: Qt.AlignTop
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/icons/editor/reset-white.png"
                }

                MyText {
                    id: resetButtonText
                    Layout.alignment: Qt.AlignTop
                    text: qsTr("Reset")
                    horizontalAlignment: Text.AlignHCenter

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true

                        onEntered: {
                            resetButtonText.color = "grey"
                        }

                        onExited: {
                            resetButtonText.color = resetButtonText.defaultColor
                        }

                        onClicked: {
                            console.log("RESET CLICKED")
                        }
                    }
                }
            }
        }


        Rectangle {
            Layout.minimumHeight: 100
            Layout.fillWidth: true
            color: "#1D1D1D"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 50
                anchors.topMargin: 0
                anchors.bottomMargin: 10

                RowLayout {
                    Layout.fillWidth: true

                    MyText {
                        text: qsTr("Yaw")
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                    }

                    MySlider {
                        id: yawSlider
                        Layout.fillWidth: true
                    }

                    MyText {
                        text: yawSlider.value.toFixed(2)
                        horizontalAlignment: Text.AlignHCenter
                    }
                }

                RowLayout {
                    Layout.fillWidth: true

                    MyText {
                        text: qsTr("Pitch")
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                    }

                    MySlider {
                        id: pitchSlider
                        Layout.fillWidth: true
                    }

                    MyText {
                        text: pitchSlider.value.toFixed(2)
                        horizontalAlignment: Text.AlignHCenter
                    }
                }

                RowLayout {
                    Layout.fillWidth: true

                    MyText {
                        text: qsTr("Roll")
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                    }

                    MySlider {
                        id: rollSlider
                        Layout.fillWidth: true
                    }

                    MyText {
                        text: rollSlider.value.toFixed(2)
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 45
            color: "#1D1D1D"

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                MyButton {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.maximumWidth: parent.width/2
                    text: qsTr("Process".toUpperCase())
                }
            }
        }
    }
}