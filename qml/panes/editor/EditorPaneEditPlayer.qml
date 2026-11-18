import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtWebEngine
import QtWebChannel

import FusionFFmpegStudio

Rectangle {
    id: root
    color: "transparent"

    /* ==================== Public API ==================== */

    // Video sources — setting both auto-loads.
    property url frontURL: ""
    property url backURL:  ""

    // Live, read-only state mirrored from the WebView.
    readonly property real currentTime:       webView.currentTime
    readonly property real totalTime:         webView.totalTime
    readonly property bool viewerInitialized: webView.viewerInitialized
    readonly property bool isPlaying:         webView.isPlaying

    // Editable selection / cut mode.
    property alias selStart:    webView.selStart
    property alias selEnd:      webView.selEnd
    property alias cuttingMode: webView.cuttingMode

    // Signals mirrored from the JS bridge.
    signal ready(var info)
    signal playingChanged(bool playing)
    signal cameraChanged(var data)
    signal profileChanged(var data)
    signal settingsChanged(var settings)

    // Process button
    property string processButtonText: qsTr("PROCESS")
    signal processRequested(var settings)

    property bool _applyingSettings:   false
    property bool _suppressAutoLoad:   false

    // Methods.
    function loadVideos(front, back) { webView.loadVideos(front, back) }
    function togglePlayback()        { webView.togglePlayback() }
    function seekToFraction(f)       { webView.seekToFraction(f) }
    function stepFrame(dir)          { webView.stepFrame(dir) }
    function setYaw(v)               { webView.setYaw(v) }
    function setPitch(v)             { webView.setPitch(v) }
    function setRoll(v)              { webView.setRoll(v) }
    function formatTime(sec)         { return webView.formatTime(sec) }

    function resetView() {
        webView.runJavaScript("window.fusionViewer && window.fusionViewer.resetView()")
        yawSlider.value   = 0
        pitchSlider.value = 0
        rollSlider.value  = 0
    }

    onFrontURLChanged: _tryLoad()
    onBackURLChanged:  _tryLoad()
    function _tryLoad() {
        if (root._suppressAutoLoad) return
        if (frontURL && backURL) loadVideos(frontURL, backURL)
    }

    onSelStartChanged:     root._scheduleSettingsEmit()
    onSelEndChanged:       root._scheduleSettingsEmit()
    onCuttingModeChanged:  root._scheduleSettingsEmit()

    Connections {
        target: webView
        function onCuttingModeChanged() { cutMode.checked = webView.cuttingMode }
    }

    Timer {
        id: settingsDebounce
        interval: 120
        onTriggered: {
            if (!root._applyingSettings)
                root.settingsChanged(root.getSettings())
        }
    }

    function _scheduleSettingsEmit() {
        if (root._applyingSettings) return
        settingsDebounce.restart()
    }

    function getSettings() {
        return {
            yaw:         yawSlider.value,
            pitch:       pitchSlider.value,
            roll:        rollSlider.value,
            cuttingMode: webView.cuttingMode,
            selStart:    webView.selStart,
            selEnd:      webView.selEnd,
            playerMode:  playerMode.checked
        }
    }

    function applySettings(settings) {
        if (!settings) return
        root._applyingSettings = true
        settingsDebounce.stop()
        try {
            if (settings.yaw         !== undefined) yawSlider.value         = settings.yaw
            if (settings.pitch       !== undefined) pitchSlider.value       = settings.pitch
            if (settings.roll        !== undefined) rollSlider.value        = settings.roll
            if (settings.cuttingMode !== undefined) webView.cuttingMode     = settings.cuttingMode
            if (settings.selStart    !== undefined) webView.selStart        = settings.selStart
            if (settings.selEnd      !== undefined) webView.selEnd          = settings.selEnd
            if (settings.playerMode  !== undefined) playerMode.checked      = settings.playerMode

            // Push camera immediately — the slider onValueChanged handlers
            // are gated on `pressed` and would otherwise skip this.
            setYaw(yawSlider.value)
            setPitch(pitchSlider.value)
            setRoll(rollSlider.value)
        } finally {
            root._applyingSettings = false
        }
    }

    function loadVideo(front, back, settings) {
        // Suppress the URL change handlers so loadVideos isn't invoked twice.
        root._suppressAutoLoad = true
        root.frontURL = front
        root.backURL  = back
        root._suppressAutoLoad = false

        loadVideos(front, back)
        if (settings) root.applySettings(settings)
    }

    /* ==================== Layout ==================== */

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // ---------- Video area ----------
        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "grey"

            WebEngineView {
                id: webView
                anchors.fill: parent
                url: "qrc:/web/videoplayer/index.html"
                settings.playbackRequiresUserGesture: false
                webChannel: WebChannel { registeredObjects: [webViewBridge] }

                property var  pendingVideos: null
                property bool viewerInitialized: false
                property real currentTime: 0
                property real totalTime: 0
                property real selStart: 0
                property real selEnd: 1
                property bool cuttingMode: false
                property bool isPlaying: false

                onSelStartChanged: pushSelection()
                onSelEndChanged:   pushSelection()

                onJavaScriptConsoleMessage: function(level, message, lineNumber, sourceID) {
                    console.log("[JS]", sourceID, ":", lineNumber, "->", message)
                }

                onCuttingModeChanged: {
                    if (viewerInitialized)
                        runJavaScript(
                            "window.fusionViewer && window.fusionViewer.setLoopActive(" + cuttingMode + ")")
                }

                function pushSelection() {
                    if (!viewerInitialized) return
                    runJavaScript(
                        "window.fusionViewer && window.fusionViewer.setSelection("
                        + selStart + "," + selEnd + ")")
                }

                function formatTime(seconds) {
                    if (!isFinite(seconds) || seconds < 0) return "00:00"
                    var m = Math.floor(seconds / 60)
                    var s = Math.floor(seconds % 60)
                    return (m < 10 ? "0" : "") + m + ":" + (s < 10 ? "0" : "") + s
                }

                function seekToFraction(fraction) {
                    if (!viewerInitialized) return
                    runJavaScript(
                        "window.fusionViewer && window.fusionViewer.seekFraction(" + fraction + ")")
                }

                function togglePlayback() {
                    runJavaScript("window.fusionViewer.togglePlayback()")
                }

                function stepFrame(direction) {
                    if (!viewerInitialized) return
                    runJavaScript(
                        "window.fusionViewer && window.fusionViewer.stepFrame(" + direction + ")")
                }

                function setYaw(v)   { if (viewerInitialized) runJavaScript("window.fusionViewer && window.fusionViewer.setYaw("   + v + ")") }
                function setPitch(v) { if (viewerInitialized) runJavaScript("window.fusionViewer && window.fusionViewer.setPitch(" + v + ")") }
                function setRoll(v)  { if (viewerInitialized) runJavaScript("window.fusionViewer && window.fusionViewer.setRoll("  + v + ")") }

                function loadVideos(frontUrl, backUrl) {
                    pendingVideos = [frontUrl, backUrl]
                    if (viewerInitialized) _doLoadVideos()
                    else console.log("loadVideos queued – waiting for viewer ready")
                }

                function _doLoadVideos() {
                    if (!pendingVideos) return
                    const frontUrl = pendingVideos[0]
                    const backUrl  = pendingVideos[1]
                    pendingVideos  = null

                    const js = `
                        (window.fusionViewer &&
                         typeof window.fusionViewer.loadVideos === 'function')
                            ? (window.fusionViewer.loadVideos(
                                    ${JSON.stringify(frontUrl)},
                                    ${JSON.stringify(backUrl)}), 'ok')
                            : 'viewer-not-ready';
                    `
                    runJavaScript(js, function(result) {
                        console.log("loadVideos() returned:", result)
                        if (result === 'viewer-not-ready') {
                            pendingVideos = [frontUrl, backUrl]
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
                            webView.pushSelection()
                            webView.runJavaScript(
                                "window.fusionViewer && window.fusionViewer.setLoopActive(" + webView.cuttingMode + ")")

                            // Re-push the camera state that applySettings() may have set
                            // while the viewer was still booting.
                            webView.setYaw(yawSlider.value)
                            webView.setPitch(pitchSlider.value)
                            webView.setRoll(rollSlider.value)
                        }
                        root.ready(data)
                    }

                    function playingChanged(playing) {
                        console.log("Playing:", playing)
                        webView.isPlaying = playing
                        root.playingChanged(playing)
                    }

                    function cameraChanged(data) {
                        if (!data) return
                        if (!yawSlider.pressed)   yawSlider.value   = data.yaw   || 0
                        if (!pitchSlider.pressed) pitchSlider.value = data.pitch || 0
                        if (!rollSlider.pressed)  rollSlider.value  = data.roll  || 0
                        root.cameraChanged(data)
                    }

                    function profileChanged(data) {
                        console.log("Active profile:", data.name)
                        root.profileChanged(data)
                    }

                    function timeChanged(data) {
                        if (!data) return
                        webView.currentTime = data.currentTime || 0
                        webView.totalTime   = data.duration    || 0
                        if (seekInteraction.dragMode !== 3 && webView.totalTime > 0) {
                            seekSlider.value = data.fraction
                        }
                    }
                }
            }
        }

        // ---------- Seek bar ----------
        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 25
            Layout.margins: 10

            MyText { text: webView.formatTime(webView.currentTime) }

            Item {
                id: seekContainer
                Layout.fillHeight: true
                Layout.fillWidth: true

                Rectangle {
                    z: 1
                    x: seekContainer.width * webView.selStart
                    width: Math.max(0, seekContainer.width * (webView.selEnd - webView.selStart))
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    color: "#5500A0FF"
                    visible: width > 0 && webView.cuttingMode
                }

                MySlider {
                    id: seekSlider
                    anchors.fill: parent
                    from: 0
                    to: 1
                    value: 0
                }

                Rectangle {
                    id: startBar
                    z: 100
                    width: 3
                    visible: webView.cuttingMode
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    x: Math.max(0, Math.min(
                        seekContainer.width * webView.selStart - width / 2,
                        seekContainer.width - width))
                    color: "#FF3355"
                }
                Rectangle {
                    id: startGrab
                    z: 101
                    visible: webView.cuttingMode
                    width: 18; height: 18; radius: 9
                    anchors.horizontalCenter: startBar.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#FF3355"
                    border.color: "#FFFFFF"
                    border.width: 2
                }

                Rectangle {
                    id: endBar
                    z: 100
                    width: 3
                    visible: webView.cuttingMode
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    x: Math.max(0, Math.min(
                        seekContainer.width * webView.selEnd - width / 2,
                        seekContainer.width - width))
                    color: "#FF3355"
                }
                Rectangle {
                    id: endGrab
                    z: 101
                    visible: webView.cuttingMode
                    width: 18; height: 18; radius: 9
                    anchors.horizontalCenter: endBar.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#FF3355"
                    border.color: "#FFFFFF"
                    border.width: 2
                }

                MouseArea {
                    id: seekInteraction
                    anchors.fill: parent
                    hoverEnabled: true
                    acceptedButtons: Qt.LeftButton

                    readonly property int grabPx:   14
                    readonly property int minGapPx: 10
                    property int dragMode: 0

                    function fracAt(mx) {
                        return Math.max(0, Math.min(1, mx / Math.max(1, width)))
                    }

                    onPressed: function(mouse) {
                        var startPx = webView.selStart * width
                        var endPx   = webView.selEnd   * width
                        var dStart  = Math.abs(mouse.x - startPx)
                        var dEnd    = Math.abs(mouse.x - endPx)

                        if (dStart < grabPx && dEnd < grabPx) {
                            dragMode = mouse.x < (startPx + endPx) / 2 ? 1 : 2
                        } else if (dStart < grabPx) {
                            dragMode = 1
                        } else if (dEnd < grabPx) {
                            dragMode = 2
                        } else {
                            var f = fracAt(mouse.x)
                            if (webView.cuttingMode &&
                                (f < webView.selStart || f > webView.selEnd)) {
                                dragMode = 0
                                return
                            }
                            dragMode = 3
                            seekSlider.value = f
                            if (webView.totalTime > 0) webView.seekToFraction(f)
                        }
                    }

                    onPositionChanged: function(mouse) {
                        if (!pressed || dragMode === 0) return
                        var frac   = fracAt(mouse.x)
                        var minGap = minGapPx / Math.max(1, width)

                        if (dragMode === 1) {
                            webView.selStart = Math.max(0, Math.min(frac, webView.selEnd - minGap))
                        } else if (dragMode === 2) {
                            webView.selEnd = Math.min(1, Math.max(frac, webView.selStart + minGap))
                        } else if (dragMode === 3) {
                            if (webView.cuttingMode) {
                                frac = Math.max(webView.selStart, Math.min(webView.selEnd, frac))
                            }
                            seekSlider.value = frac
                            if (webView.totalTime > 0) webView.seekToFraction(frac)
                        }
                    }

                    onReleased: dragMode = 0
                    onDoubleClicked: { webView.selStart = 0; webView.selEnd = 1 }

                    cursorShape: {
                        if (!containsMouse) return Qt.ArrowCursor
                        var startPx = webView.selStart * seekContainer.width
                        var endPx   = webView.selEnd   * seekContainer.width
                        if (Math.abs(mouseX - startPx) < grabPx ||
                            Math.abs(mouseX - endPx)   < grabPx) return Qt.SizeHorCursor
                        if (webView.cuttingMode &&
                            (mouseX < startPx || mouseX > endPx)) return Qt.ForbiddenCursor
                        return Qt.PointingHandCursor
                    }
                }

                Rectangle {
                    z: 200
                    visible: seekInteraction.containsMouse && webView.totalTime > 0
                             && !(webView.cuttingMode &&
                                  (seekInteraction.mouseX < seekContainer.width * webView.selStart ||
                                   seekInteraction.mouseX > seekContainer.width * webView.selEnd))
                    x: Math.max(0, Math.min(
                        seekInteraction.mouseX - width / 2,
                        seekContainer.width - width))
                    y: -height - 4
                    width: tooltipText.implicitWidth + 10
                    height: tooltipText.implicitHeight + 6
                    color: "#222222"
                    radius: 2
                    border.color: "#F6FBF9"
                    border.width: 1

                    Text {
                        id: tooltipText
                        anchors.centerIn: parent
                        color: "#F6FBF9"
                        font.pointSize: 10
                        text: webView.formatTime(
                            (seekInteraction.mouseX
                             / Math.max(1, seekContainer.width)) * webView.totalTime)
                    }
                }
            }

            MyText { text: webView.formatTime(webView.totalTime) }
        }

        // ---------- Frame controls ----------
        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 20
            Layout.margins: 10
            spacing: 20

            Item { Layout.fillHeight: true; Layout.fillWidth: true }

            Item {
                id: prevBtn
                Layout.fillHeight: true
                Layout.preferredWidth: 40
                property color color: "#F6FBF9"

                RowLayout {
                    anchors.fill: parent
                    spacing: 2
                    Triangle { Layout.fillHeight: true; Layout.preferredWidth: 25
                               color: prevBtn.color; direction: Triangle.Left }
                    Rectangle { Layout.fillHeight: true; Layout.preferredWidth: 5
                                color: prevBtn.color }
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: prevBtn.color = "#00A0FF"
                    onExited:  prevBtn.color = "#F6FBF9"
                    onClicked: webView.stepFrame(-1)
                }
            }

            Item {
                id: pauseBtn
                visible: webView.isPlaying
                Layout.fillHeight: true
                Layout.preferredWidth: 24
                property color color: "#F6FBF9"

                RowLayout {
                    anchors.fill: parent
                    spacing: 2
                    Rectangle { Layout.fillHeight: true; Layout.preferredWidth: 5
                                color: pauseBtn.color }
                    Rectangle { Layout.fillHeight: true; Layout.preferredWidth: 5
                                color: pauseBtn.color }
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: pauseBtn.color = "#00A0FF"
                    onExited:  pauseBtn.color = "#F6FBF9"
                    onClicked: webView.togglePlayback()
                }
            }

            Item {
                id: playBtn
                visible: !webView.isPlaying
                Layout.fillHeight: true
                Layout.preferredWidth: 25
                property color color: "#F6FBF9"

                Triangle {
                    anchors.fill: parent
                    color: playBtn.color
                    direction: Triangle.Right
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: playBtn.color = "#00A0FF"
                    onExited:  playBtn.color = "#F6FBF9"
                    onClicked: webView.togglePlayback()
                }
            }

            Item {
                id: nextBtn
                Layout.fillHeight: true
                Layout.preferredWidth: 40
                property color color: "#F6FBF9"

                RowLayout {
                    anchors.fill: parent
                    spacing: 2
                    Rectangle { Layout.fillHeight: true; Layout.preferredWidth: 5
                                color: nextBtn.color }
                    Triangle { Layout.fillHeight: true; Layout.preferredWidth: 25
                               color: nextBtn.color; direction: Triangle.Right }
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: nextBtn.color = "#00A0FF"
                    onExited:  nextBtn.color = "#F6FBF9"
                    onClicked: webView.stepFrame(1)
                }
            }

            Item { Layout.fillHeight: true; Layout.fillWidth: true }
        }

        // ---------- 360 / OVERCAPTURE / CUT MODE ----------
        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 50
            color: "#222222"

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10

                MyText { text: "360";         color: playerMode.checked ? "#879899" : "#00A1E1" }
                MySwitch {
                    id: playerMode
                    knobMargin: 1
                    knobColorOff: "#00A1E1"
                    knobColorOn:  "#00A1E1"
                    trackColorOff: "transparent"
                    trackColorOn:  "transparent"
                    trackBorderWidth: 3
                    trackBorderColorOn:  "#F6FBF9"
                    trackBorderColorOff: "#F6FBF9"
                    onCheckedChanged: root._scheduleSettingsEmit()
                }
                MyText { text: "OVERCAPTURE"; color: playerMode.checked ? "#00A1E1" : "#879899" }

                Item { Layout.fillWidth: true }
                MyText { text: qsTr("CUT MODE"); color: "#F6FBF9" }
                MySwitch {
                    id: cutMode
                    Layout.fillHeight: true
                    checked: webView.cuttingMode          // reflect external changes
                    onToggled: webView.cuttingMode = checked
                }
            }
        }

        // ---------- Reset ----------
        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 50
            color: "#1D1D1D"

            RowLayout {
                anchors.fill: parent
                anchors.margins: 20

                Item { Layout.fillHeight: true; Layout.fillWidth: true }

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
                        onEntered: resetButtonText.color = "grey"
                        onExited:  resetButtonText.color = resetButtonText.defaultColor
                        onClicked: root.resetView()
                    }
                }
            }
        }

        // ---------- Yaw / Pitch / Roll ----------
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
                    MyText { Layout.minimumWidth: 50; text: qsTr("Yaw"); font.bold: true
                             horizontalAlignment: Text.AlignHCenter }
                    MySlider {
                        id: yawSlider
                        Layout.fillWidth: true
                        from: -Math.PI
                        to:    Math.PI
                        onValueChanged: {
                            if (pressed) webView.setYaw(value)
                            root._scheduleSettingsEmit()
                        }
                    }
                    MyText { Layout.minimumWidth: 50
                             text: (yawSlider.value * 180 / Math.PI).toFixed(1) + "°"
                             horizontalAlignment: Text.AlignHCenter }
                }

                RowLayout {
                    Layout.fillWidth: true
                    MyText { Layout.minimumWidth: 50; text: qsTr("Pitch"); font.bold: true
                             horizontalAlignment: Text.AlignHCenter }
                    MySlider {
                        id: pitchSlider
                        Layout.fillWidth: true
                        from: -(Math.PI / 2 - 0.01)
                        to:   (Math.PI / 2 - 0.01)
                        onValueChanged: {
                            if (pressed) webView.setPitch(value)
                            root._scheduleSettingsEmit()
                        }
                    }
                    MyText { Layout.minimumWidth: 50
                             text: (pitchSlider.value * 180 / Math.PI).toFixed(1) + "°"
                             horizontalAlignment: Text.AlignHCenter }
                }

                RowLayout {
                    Layout.fillWidth: true
                    MyText { Layout.minimumWidth: 50; text: qsTr("Roll"); font.bold: true
                             horizontalAlignment: Text.AlignHCenter }
                    MySlider {
                        id: rollSlider
                        Layout.fillWidth: true
                        from: -Math.PI
                        to:    Math.PI
                        onValueChanged: {
                            if (pressed) webView.setRoll(value)
                            root._scheduleSettingsEmit()
                        }
                    }
                    MyText { Layout.minimumWidth: 50
                             text: (rollSlider.value * 180 / Math.PI).toFixed(1) + "°"
                             horizontalAlignment: Text.AlignHCenter }
                }
            }
        }

        // ---------- Footer buttons ----------
        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: 45
            color: "#1D1D1D"

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10

                Item { Layout.fillHeight: true; Layout.fillWidth: true }

                MyButton {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.maximumWidth: parent.width / 2
                    text: root.processButtonText
                    onClicked: root.processRequested(root.getSettings())
                }
            }
        }
    }
}