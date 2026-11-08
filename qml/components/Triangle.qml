
import QtQuick
import QtQuick.Shapes

Shape {
    id: root

    enum Direction {
        Up,
        Down,
        Left,
        Right
    }

    property int direction: Triangle.Up

    property color color: "steelblue"
    property color borderColor: "transparent"
    property real borderWidth: 0

    preferredRendererType: Shape.CurveRenderer


    ShapePath {
        id: trianglePath

        strokeColor: root.borderColor
        strokeWidth: root.borderWidth
        fillColor: root.color
        joinStyle: ShapePath.MiterJoin

        startX: root.direction === Triangle.Left ? 0
              : root.direction === Triangle.Right ? root.width
              : root.width / 2

        startY: root.direction === Triangle.Up ? 0
              : root.direction === Triangle.Down ? root.height
              : root.height / 2

        PathLine {
            x: root.direction === Triangle.Up || root.direction === Triangle.Down
               ? root.width
               : root.direction === Triangle.Left ? root.width : 0

            y: root.direction === Triangle.Up || root.direction === Triangle.Down
               ? (root.direction === Triangle.Up ? root.height : 0)
               : root.height
        }

        PathLine {
            x: root.direction === Triangle.Up || root.direction === Triangle.Down
               ? 0
               : root.direction === Triangle.Left ? root.width : 0

            y: root.direction === Triangle.Up || root.direction === Triangle.Down
               ? (root.direction === Triangle.Up ? root.height : 0)
               : 0
        }

        PathLine {
            x: trianglePath.startX
            y: trianglePath.startY
        }
    }
}
