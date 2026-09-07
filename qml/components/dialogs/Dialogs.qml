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
        height: 200

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