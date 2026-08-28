import QtQuick 2.15
import QtQuick.Controls 2.15
import FusionFFmpegStudio 1.0

Window {
    visible: true
    width: 800
    height: 600
    title: "Fusion FFmpeg Studio"

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: WelcomePane {}
    }

    Connections {
        target: mainController

        function onNavigateTo(pane) {
            switch (pane) {
                case MainController.Pane.About:
                    console.log("Navigate to About Pane")
                    break
                default:
                    console.log("Unknown Pane")
            }
            //stack.push(Qt.resolvedUrl(pane + ".qml"))
        }
    }
}
