import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtWebEngine
import QtWebChannel

import FusionFFmpegStudio

Rectangle {
    required property var controller
    property string disableMessage: "MEDIA\n NOT\n FOUND"
    property bool disabled: false

    color: "#2e2e2e"

    function setSource(source) {
        disabled = false
        console.log("source="+source)
    }

    function setVideo(video) {
        /*
        const base = "http://localhost:15000/video?path=";
        webView.loadVideos(
            base + encodeURIComponent(video.front),
            base + encodeURIComponent(video.back)
        );
        */
        webView.loadVideos(video.frontURL, video.backURL);
    }

    ColumnLayout {
        anchors.fill: parent
        id: videoPlayerLayout

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: disabled

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "black"

                WebEngineView {
                    id: webView
                    anchors.fill: parent
                    url: "qrc:/web/videoplayer/index.html"
                    webChannel: WebChannel {
                        registeredObjects: [bridge]
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
                }

                QtObject {
                    id: bridge
                    WebChannel.id: "fusionViewerBridge"

                    function ready(data) {
                        console.log("Viewer ready:", JSON.stringify(data))
                        if (data && data.initialized === true) {
                            webView.viewerInitialized = true
                            webView._doLoadVideos()               // <-- replay queued call
                        }
                        // The second ready() carries video metadata; ignore if not needed
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

                Shortcut {
                    sequence: "Space"
                    onActivated: webView.runJavaScript("window.fusionViewer.togglePlayback()")
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "black"

                Text {
                    anchors.fill: parent
                    text: disableMessage
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    fontSizeMode: Text.Fit
                    minimumPointSize: 10
                    font.pointSize: 500
                    color: "#FFFFFF"
                }
            }
        }
    }
}
