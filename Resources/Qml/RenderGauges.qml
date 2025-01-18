import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Rectangle {
    color: "#353637"

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            Speedometer {
                id: renderPercentGauge
                Layout.fillHeight: true
                Layout.fillWidth: true

                property double percent: 0.0

                Label {
                    anchors.centerIn: parent
                    font.pointSize: 26
                    color: "#FEFEFE"
                    text: renderPercentGauge.percent + "%"
                }
            }

            Speedometer {
                id: renderSpeedGauge
                Layout.fillHeight: true
                Layout.fillWidth: true
                value: handlePage.newVal
                onValueChanged: handlePage.newVal = value
                startAngle: 40
                endAngle: 320
                rotation: 180
                trackWidth: 5
                progressWidth: 20
                minValue: 0
                maxValue: 10
                progressColor: "#50FA7B"
                capStyle: Qt.FlatCap

                property double timesX: 0.0

                handle: Rectangle {
                    transform: Translate {
                        x: (renderSpeedGauge.handleWidth - width) / 2
                        y: renderSpeedGauge.handleHeight / 2
                    }

                    width: 10
                    height: renderSpeedGauge.height / 2
                    color: "#FFac89"
                    radius: width / 2
                    antialiasing: true
                }

                Label {
                    anchors.centerIn: parent
                    anchors.verticalCenterOffset: -90
                    rotation: 180
                    font.pointSize: 26
                    color: "#FEFEFE"
                    text: renderSpeedGauge.timesX + "x"
                }

            }

        }
    }
}
