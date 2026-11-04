import QtQuick 2.15
import QtQuick.Controls 2.15

import FusionFFmpegStudio

Window {
    id: mainWindow
    visible: true
    width: 800
    height: 600
    title: "Fusion FFmpeg Studio"

    property double resizeAnimationTime: 350

    Behavior on width {
        NumberAnimation {
            duration: resizeAnimationTime
            easing.type: Easing.InOutCubic
        }
    }

    Behavior on height {
        NumberAnimation {
            duration: resizeAnimationTime
            easing.type: Easing.InOutCubic
        }
    }

    Behavior on x {
        NumberAnimation {
            duration: resizeAnimationTime
            easing.type: Easing.InOutCubic
        }
    }

    Behavior on y {
        NumberAnimation {
            duration: resizeAnimationTime
            easing.type: Easing.InOutCubic
        }
    }

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: Welcome {}

        function windowMaximize() {
            windowResize(
                mainWindow.screen.desktopAvailableWidth,
                mainWindow.screen.desktopAvailableHeight
            )
        }

        function windowFullscreen() {
            mainWindow.showFullScreen()
        }

        function windowResizePercent(widthPercent, heightPercent) {
            if (!widthPercent || widthPercent <= 0 || widthPercent > 100 ||
                !heightPercent || heightPercent <= 0 || heightPercent > 100) {
                console.error("Could not resize main window: Invalid percent dimensions: " + widthPercent + "x" + heightPercent)
            }
            windowResize(
                mainWindow.screen.desktopAvailableWidth * (widthPercent/100),
                mainWindow.screen.desktopAvailableHeight * (heightPercent/100)
            )
        }

        function windowResize(width, height) {
            if (width <= 0 || height <= 0) {
                console.error("Could not resize main window: Invalid dimensions: " + width + "x" + height)
                return
            }

            console.debug("Resizing main window to " + width + "x" + height)

            const screen = mainWindow.screen

            const availableWidth = screen.desktopAvailableWidth
            const availableHeight = screen.desktopAvailableHeight

            const targetWidth = Math.min(width, availableWidth)
            const targetHeight = Math.min(height, availableHeight)

            mainWindow.width = targetWidth
            mainWindow.height = targetHeight

            // Center window on the screen
            mainWindow.x = screen.virtualX + (screen.width - targetWidth) / 2
            mainWindow.y = screen.virtualY + (screen.height - targetHeight) / 2
        }
    }

    Connections {
        target: mainController

        function onNavigateTo(pane, attributes) {
            let paneUrl = mainController.paneToUrl(pane)
            if (paneUrl === "") {
                console.error("Unknown pane: " + pane)
                return
            }
            stack.push(paneUrl, attributes)
        }

        function onAddWindow(window, modal) {
            let windowUrl = mainController.windowToUrl(window)
            if (windowUrl === "") {
                console.error("Unknown window: " + window)
                return
            }

            let component = Qt.createComponent(windowUrl)

            if (component.status !== Component.Ready) {
                console.error(
                    "Failed to create window:",
                    windowUrl,
                    "\n",
                    component.errorString()
                )
                return
            }

            let windowObject = component.createObject(null, {
                "parentWindow": mainWindow
            })

            if (!windowObject) {
                console.error("Failed to instantiate:", windowUrl)
                return
            }

            windowObject.transientParent = mainWindow
            windowObject.modality = modal
                ? Qt.WindowModal
                : Qt.NonModal

            windowObject.show()
        }

        function onBack() {
            stack.pop()
        }
    }
}
