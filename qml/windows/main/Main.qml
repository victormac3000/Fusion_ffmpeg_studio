import QtQuick 2.15
import QtQuick.Controls 2.15

import FusionFFmpegStudio

Window {
    id: mainWindow
    visible: true

    width: 800
    minimumWidth: 600

    height: 600
    minimumHeight: 400

    title: "Fusion FFmpeg Studio"

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: Welcome {}
    }

    Connections {
        target: mainController

        function onNavigateTo(pane) {
            let paneUrl = mainController.paneToUrl(pane)
            if (paneUrl === "") {
                console.error("Unknown pane: " + pane)
                return
            }
            stack.push(paneUrl)
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
