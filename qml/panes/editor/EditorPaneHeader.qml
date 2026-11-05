import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: root
    color: "#141414"

    property int renderItems: 1

    RowLayout {
        id: headerLayout
        anchors.fill: parent
        anchors.margins: 10

        Image {
            Layout.preferredHeight: headerLayout.height
            Layout.preferredWidth: headerLayout.height
            fillMode: Image.PreserveAspectFit
            source: "qrc:/images/AppIcon.png"
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        TabBar {
            Layout.preferredWidth: 300
            Layout.fillHeight: true
            contentHeight: height
            spacing: 0

            TabButton {
                text: qsTr("EDIT")
                Layout.fillWidth: true
                Layout.fillHeight: true
                background: Rectangle {
                    border.color: "#666666"
                    border.width: 2
                    color: parent.checked ? "#666666" : "#141414"
                }
            }

            TabButton {
                id: renderTabButton
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: qsTr("RENDER")
                background: Rectangle {
                    border.color: "#666666"
                    border.width: 2
                    color: parent.checked ? "#666666" : "#141414"

                    RowLayout {
                        anchors.fill: parent
                        visible: root.renderItems > 0

                        Rectangle {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignRight
                            Layout.margins: 10

                            Layout.maximumHeight: 20
                            Layout.maximumWidth: Layout.maximumHeight

                            color: "#00C1F5"
                            radius: 180


                            Text {
                                color: "#FFFFFF"
                                anchors.fill: parent
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                text: root.renderItems
                            }
                        }
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
