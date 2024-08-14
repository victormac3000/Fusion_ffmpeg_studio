import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Rectangle {
    color: "lightgrey"

    Constants {
        id: constants
        visible: false
    }

    ColumnLayout {
        width: parent.width
        height: parent.height

        GridLayout {
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
            Layout.fillHeight: true
            Layout.fillWidth: true
            columns: 4

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.maximumWidth: parent.width*0.15

                border.color: "green"
                border.width: 3

                Text {
                    anchors.fill: parent
                    font.pointSize: 16
                    clip: true
                    font.family: "Arial"
                    text: "Codec"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    padding: 5
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.minimumHeight: 100
                Layout.columnSpan: 2
                border.color: "green"
                border.width: 3

                ScrollView {
                    anchors.fill: parent
                    anchors.margins: 10
                    contentHeight: codecsDescriptionText.height

                    TextArea {
                        id: codecsDescriptionText
                        font.pointSize: 16
                        clip: true
                        font.family: "Arial"
                        text: ""
                        wrapMode: Text.WordWrap
                        readOnly: true
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignTop
                        padding: 5
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.maximumWidth: parent.width*0.25
                border.color: "green"
                border.width: 3

                MyComboBox {
                    signal codecChanged()

                    id: codecsComboBox
                    objectName: "codecsComboBox"
                    anchors.fill: parent
                    anchors.margins: 10
                    font.pointSize: 16

                    onCurrentIndexChanged: {
                        var selectedCodec = model.get(currentIndex).text
                        save(selectedCodec, "codec")
                        var info = constants.codecsInfo
                        codecsDescriptionText.text = ""
                        if (selectedCodec in info) {
                            codecsDescriptionText.text = info[selectedCodec]
                        }
                        codecChanged()
                    }
                }
            }


            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.maximumWidth: parent.width*0.15
                border.color: "green"
                border.width: 3

                Text {
                    anchors.fill: parent
                    font.pointSize: 16
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
                Layout.alignment: Qt.AlignTop
                Layout.minimumHeight: 100
                border.color: "green"
                border.width: 3

                ScrollView {
                    anchors.fill: parent
                    anchors.margins: 10
                    contentHeight: encodersDescriptionText.height

                    TextArea {
                        id: encodersDescriptionText
                        clip: true
                        font.pointSize: 16
                        font.family: "Arial"
                        text: ""
                        readOnly: true
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignTop
                        padding: 5
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.minimumHeight: 100
                border.color: "green"
                border.width: 3

                Image {
                    id: acceleratorImage
                    source: hardware ? "Images/gpu_icon.png" : "Images/cpu_icon.png"
                    fillMode: Image.PreserveAspectFit
                    width: parent.width
                    height: parent.height
                    property bool hardware: false
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.maximumWidth: parent.width*0.25
                border.color: "green"
                border.width: 3

                MyComboBox {
                    id: encodersComboBox
                    objectName: "encodersComboBox"
                    anchors.fill: parent
                    anchors.margins: 10
                    font.pointSize: 16

                    onCurrentIndexChanged: {
                        if (currentIndex >= 0) {
                            var selectedEncoder = model.get(currentIndex).text
                            save(selectedEncoder, "encoder")
                            var info = constants.encodersInfo
                            encodersDescriptionText.text = ""
                            if (selectedEncoder in info) {
                                if ("description" in info[selectedEncoder]) {
                                    encodersDescriptionText.text = info[selectedEncoder]["description"]
                                }
                                if ("hardware" in info[selectedEncoder]) {
                                    acceleratorImage.hardware = info[selectedEncoder]["hardware"];
                                }
                            }
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.maximumWidth: parent.width*0.15
                border.color: "green"
                border.width: 3

                Text {
                    anchors.fill: parent
                    font.pointSize: 16
                    font.family: "Arial"
                    text: "Format"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    padding: 5
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.minimumHeight: 100
                Layout.columnSpan: 2
                border.color: "green"
                border.width: 3

                ScrollView {
                    anchors.fill: parent
                    anchors.margins: 10
                    contentHeight: formatsDescriptionText.height

                    TextArea {
                        id: formatsDescriptionText
                        clip: true
                        font.pointSize: 16
                        font.family: "Arial"
                        text: ""
                        readOnly: true
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignTop
                        padding: 5
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.maximumWidth: parent.width*0.25
                border.color: "green"
                border.width: 3

                MyComboBox {
                    id: formatsComboBox
                    objectName: "formatsComboBox"
                    anchors.fill: parent
                    anchors.margins: 10
                    font.pointSize: 16

                    onCurrentIndexChanged: {
                        if (currentIndex >= 0) {
                            var selectedFormat = model.get(currentIndex).text
                            save(selectedFormat, "format")
                            var info = constants.formatsInfo
                            formatsDescriptionText.text = ""
                            if (selectedFormat in info) {
                                formatsDescriptionText.text = info[selectedFormat]
                            }
                        }
                    }
                }
            }
        }
    }
}
