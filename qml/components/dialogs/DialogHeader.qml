import QtQuick
import QtQuick.Layouts

ColumnLayout {
    id: root
    implicitHeight: 40
    spacing: 0

    required property var dialog
    property string type: dialog.dialogType

    property string titleMessage: {
        if (type == "critical") return "Critical error"
        if (type == "info") return "Information"
        if (type == "warning") return "Warning"
        if (type == "question") return "Choose"
        if (type == "questionw") return "Choose"
        return "Generic title"
    }

    property string iconPath: {
        var basePath = "qrc:/icons/dialogs"
        if (type == "critical") return basePath + "/critical.png"
        if (type == "info") return basePath + "/info.png"
        if (type == "warning") return basePath + "/warning.png"
        if (type == "question") return basePath + "/question.png"
        if (type == "questionw") return basePath + "/warning.png"
        return ""
    }

    property color hrColor: {
        if (type == "critical") return "red"
        if (type == "info") return "green"
        if (type == "warning") return "yellow"
        if (type == "question") return "blue"
        if (type == "questionw") return "yellow"
        return "blue"
    }

    RowLayout {
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.margins: 15

        Image {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredHeight: 45
            Layout.preferredWidth: 45
            Layout.maximumHeight: 45
            Layout.maximumWidth: 45
            source: root.iconPath
        }

        Text {
            Layout.fillHeight: true
            Layout.fillWidth: true
            text: root.titleMessage
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Image {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredHeight: 45
            Layout.preferredWidth: 45
            Layout.maximumHeight: 45
            Layout.maximumWidth: 45
            source: root.iconPath
        }
    }

    Rectangle {
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.preferredHeight: 3
        Layout.minimumHeight: 3
        Layout.maximumHeight: 3
        color: hrColor
        border.width: 0
    }
}
