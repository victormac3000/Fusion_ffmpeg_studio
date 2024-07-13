import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Rectangle {
    color: "lightgrey"

    signal requestEncoders(codec: string)
    signal requestVideoFormats(codec: string)
    signal save(data: string, type: string)


    function addCodecs(list, defaultCodec) {
        codecsComboBox.model.clear()
        var defaultCodecIndex = 0;
        for (var i=0; i<list.length; i++) {
            if (list[i] === defaultCodec) {
                defaultCodecIndex = i;
            }
            codecsComboBox.model.append({text: list[i]})
        }

        codecsComboBox.currentIndex = defaultCodecIndex
        codecsComboBox.currentIndexChanged()
    }

    function addEncoders(list, defaultEnconder) {
        encodersComboBox.model.clear()
        var defaultEncoderIndex = 0;
        for (var i=0; i<list.length; i++) {
            if (list[i] === defaultEnconder) {
                defaultEncoderIndex = i;
            }
            encodersComboBox.model.append({text: list[i]})
        }
        encodersComboBox.currentIndex = defaultEncoderIndex
    }

    function addFormats(list, defaultFormat) {
        formatsComboBox.model.clear()
        var defaultComboBoxIndex = 0;
        for (var i=0; i<list.length; i++) {
            if (list[i] === defaultFormat) {
                defaultComboBoxIndex = i;
            }
            formatsComboBox.model.append({text: list[i]})
        }
        formatsComboBox.currentIndex = defaultComboBoxIndex
    }

    ColumnLayout {
        width: parent.width
        height: parent.height

        GridLayout {
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.columnSpan: 2
            columns: 3

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: 50
                Layout.alignment: Qt.AlignTop
                border.color: "green"
                border.width: 3

                Text {
                    anchors.fill: parent
                    font.pointSize: 20
                    minimumPointSize: 10
                    fontSizeMode: Text.Fit
                    font.family: "Arial"
                    text: "Output video codec"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    padding: 5
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: 50
                Layout.alignment: Qt.AlignTop
                border.color: "green"
                border.width: 3
                Layout.columnSpan: 2

                ComboBox {
                    id: codecsComboBox
                    anchors.fill: parent
                    anchors.margins: 10
                    font.pointSize: 20
                    model: ListModel {}
                    currentIndex: 0

                    onCurrentIndexChanged: {
                        save(model.get(currentIndex).text, "codec")
                        requestEncoders(model.get(currentIndex).text)
                        requestVideoFormats(model.get(currentIndex).text)
                    }
                }
            }


            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: 50
                Layout.alignment: Qt.AlignTop
                border.color: "green"
                border.width: 3

                Text {
                    anchors.fill: parent
                    font.pointSize: 20
                    minimumPointSize: 10
                    fontSizeMode: Text.Fit
                    font.family: "Arial"
                    text: "Encoder"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    padding: 5
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: 50
                Layout.alignment: Qt.AlignTop
                border.color: "green"
                border.width: 3

                Text {
                    id: encoderDescriptionText
                    anchors.fill: parent
                    anchors.margins: 10
                    font.pointSize: 20
                    text: ""
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: 50
                Layout.alignment: Qt.AlignTop
                border.color: "green"
                border.width: 3

                ComboBox {
                    id: encodersComboBox
                    anchors.fill: parent
                    anchors.margins: 10
                    font.pointSize: 20
                    model: ListModel {}
                    currentIndex: 0

                    onCurrentIndexChanged: {
                        if (currentIndex >= 0) {
                            save(model.get(currentIndex).text, "encoder")
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: 50
                Layout.alignment: Qt.AlignTop
                border.color: "green"
                border.width: 3

                Text {
                    anchors.fill: parent
                    font.pointSize: 20
                    minimumPointSize: 10
                    fontSizeMode: Text.Fit
                    font.family: "Arial"
                    text: "Video format"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    padding: 5
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: 50
                Layout.alignment: Qt.AlignTop
                border.color: "green"
                border.width: 3
                Layout.columnSpan: 2

                ComboBox {
                    id: formatsComboBox
                    anchors.fill: parent
                    anchors.margins: 10
                    font.pointSize: 20
                    model: ListModel {}
                    currentIndex: 0

                    onCurrentIndexChanged: {
                        if (currentIndex >= 0) {
                            save(model.get(currentIndex).text, "format")
                        }
                    }
                }
            }
        }
    }
}
