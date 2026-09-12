import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import FusionFFmpegStudio

Item {
    id: root

    Dialog {
        id: rootDialog
        modal: true
        closePolicy: Popup.NoAutoClose
        width: 400

        property string dialogType: "critical"

        anchors.centerIn: Overlay.overlay

        header: DialogHeader {
            dialog: rootDialog
        }

        contentItem: DialogContent {
            message: ""
        }

        footer: DialogFooter {
            dialog: rootDialog
        }
    }

    Dialog {
        id: spinnerDialog
        modal: true
        closePolicy: Popup.NoAutoClose
        height: 200
        width: 400

        anchors.centerIn: Overlay.overlay

        header: null
        footer: null
        contentItem: DialogSpinnerContent{}
    }

    function spinner(message) {
        spinnerDialog.contentItem.message = message
        spinnerDialog.open()
        return spinnerDialog
    }

    function create(type, message, title, callback) {
        rootDialog.dialogType = type
        rootDialog.contentItem.message = message
        if (title) {
            rootDialog.header.titleMessage = title
        }
        if (callback) {
            rootDialog.footer.callback = callback
        }
        rootDialog.open()
    }

    function question(message, title, callback) {
       create("question", message, title, callback)
    }

    function questionw(message, title, callback) {
       create("questionw", message, title, callback)
    }

    function info(message, title, callback) {
        create("info", message, title, callback)
    }

    function warning(message, title, callback) {
        create("warning", message, title, callback)
    }

    function error(message, title, callback) {
        create("critical", message, title, callback)
    }
}