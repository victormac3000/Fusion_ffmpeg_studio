import QtQuick

Rectangle {
    color: "green"

    MouseArea {
        property bool isClicking: false

        anchors.fill: parent
        onClicked: {
            isClicking: true
        }
    }


}
