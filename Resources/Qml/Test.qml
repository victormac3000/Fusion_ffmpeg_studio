import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Flickable {
    width: 1280
    height: 720
    flickableDirection: Flickable.VerticalFlick
    boundsBehavior: Flickable.DragOverBounds
    clip: true
    contentHeight: recentProjectsLayout.height


    ColumnLayout {
        id: recentProjectsLayout
        anchors.fill: parent

        Component.onCompleted: {

            for (var i=0; i<10; i++) {
                addRecentProject("Project", "/test/x.ffs", new Date())
            }

        }

        function addRecentProject(name, path) {
            var recentProjectComponent = Qt.createComponent("RecentProject.qml")
            var recentProjectElement = recentProjectComponent.createObject(recentProjectsLayout)
            recentProjectElement.height = 50
            recentProjectElement.name = name
            recentProjectElement.path = path
            recentProjectElement.rectColor = "red"
        }
    }


    ScrollBar.vertical: ScrollBar {
        policy: ScrollBar.AlwaysOn
    }
}
