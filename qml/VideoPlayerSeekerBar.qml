import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    property string barColor: "#ffffff"
    property string selectedBarColor: "blue"
    property double rangeFrom: 0
    property double rangeTo: 100
    property double barValue: 50
    property double cutStartValue: cutStartHandle.value
    property double cutEndValue: cutEndHandle.value
    property double startOffset: 0
    property double endOffset: 0
    property bool cutMode: false

    color: barColor
    radius: 40

    Rectangle {
        id: fillerRectangle
        radius: parent.radius
        color: parent.selectedBarColor
        x: 0
        y: parent.y
        height: parent.height
        width: seekerHandle.x + seekerHandle.width
    }

    Rectangle {
        property int circleRadius: parent.height*0.9
        id: seekerHandle
        radius: 180
        color: "#4abfcf"
        height: circleRadius
        width: circleRadius
        x: {
            let proposedX = (parent.width*parent.barValue)/parent.rangeTo
            if ((proposedX+width) > (parent.width+parent.endOffset)) {
                return parent.width - width - parent.endOffset
            }
            if (proposedX < parent.startOffset) {
                return parent.startOffset
            }
            return proposedX
        }
        y: (parent.height/2) - height/2
    }

    Rectangle {
        property double value: parent.rangeFrom+(parent.rangeTo-parent.rangeFrom)*0.2

        onValueChanged: {
            console.log("cutStartValue:", value)
        }

        id: cutStartHandle
        visible: cutMode
        color: "transparent"
        height: seekerHandle.height*1.5
        width: seekerHandle.width/2
        x: {
            let proposedX = (parent.width*value)/parent.rangeTo
            if ((proposedX+width) > (parent.width+parent.endOffset)) {
                return parent.width - width - parent.endOffset
            }
            if (proposedX < parent.startOffset) {
                return parent.startOffset
            }
            return proposedX
        }
        y: (parent.height/2) - height/4

        Image {
            anchors.fill: parent
            source: "Images/red_triangle.svg"
            fillMode: Image.Stretch
        }
    }

    Rectangle {
        property double value: parent.rangeFrom+(parent.rangeTo-parent.rangeFrom)*0.8

        onValueChanged: {
            console.log("cutEndValue:", value)
        }

        id: cutEndHandle
        visible: cutMode
        color: "transparent"
        height: seekerHandle.height*1.5
        width: seekerHandle.width/2
        x: {
            let proposedX = (parent.width*value)/parent.rangeTo
            if ((proposedX+width) > (parent.width+parent.endOffset)) {
                return parent.width - width - parent.endOffset
            }
            if (proposedX < parent.startOffset) {
                return parent.startOffset
            }
            return proposedX
        }
        y: (parent.height/2) - height/4

        Image {
            anchors.fill: parent
            source: "Images/red_triangle.svg"
            fillMode: Image.Stretch
        }
    }

    onBarValueChanged: {
        //console.log("value:", value)
    }

    MouseArea {
        anchors.fill: seekerHandle
        property bool isPressing: false

        onPressed: (mouse) => {
            isPressing = true
        }

        onReleased: (mouse) => {
            isPressing = false
        }

        onMouseXChanged: (mouse) => {
            if (!isPressing) return
            let newX = (mouse.x + x)
            let newValue = (newX*parent.rangeTo) / parent.width
            if (newValue > parent.rangeTo) {
                newValue = parent.rangeTo
            }
            if (newValue < parent.rangeFrom) {
                newValue = parent.rangeFrom
            }
            parent.barValue = newValue
        }
    }

    MouseArea {
        z: -1
        anchors.fill: cutStartHandle
        visible: cutMode
        property bool isPressing: false

        onPressed: (mouse) => {


            isPressing = true
        }

        onReleased: (mouse) => {
            isPressing = false
        }

        onMouseXChanged: (mouse) => {
            if (!isPressing) return
            let newX = mouse.x + x
            let newValue = (newX*parent.rangeTo) / parent.width
            if (newValue > parent.rangeTo) {
                newValue = parent.rangeTo
            }
            if (newValue < parent.rangeFrom) {
                newValue = parent.rangeFrom
            }
            let proposedX = (parent.width*newValue)/parent.rangeTo
            if ((proposedX+cutStartHandle.width) > cutEndHandle.x) {
                return
            }
            cutStartHandle.value = newValue
        }
    }

    MouseArea {
        z: -1
        anchors.fill: cutEndHandle
        visible: cutMode
        property bool isPressing: false

        onPressed: (mouse) => {
            isPressing = true
        }

        onReleased: (mouse) => {
            isPressing = false
        }

        onMouseXChanged: (mouse) => {
            if (!isPressing) return
            let newX = mouse.x + x
            let newValue = (newX*parent.rangeTo) / parent.width
            if (newValue > parent.rangeTo) {
                newValue = parent.rangeTo
            }
            if (newValue < parent.rangeFrom) {
                newValue = parent.rangeFrom
            }
            let proposedX = (parent.width*newValue)/parent.rangeTo
            if ((proposedX-cutEndHandle.width) < cutStartHandle.x) {
                return
            }
            cutEndHandle.value = newValue
        }
    }


    MouseArea {
        z: -3
        anchors.fill: parent
        onClicked: (mouse) => {
            parent.barValue = (mouse.x*parent.rangeTo)/parent.width
        }
    }
}


