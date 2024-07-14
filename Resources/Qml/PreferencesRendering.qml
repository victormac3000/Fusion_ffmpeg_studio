import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Rectangle {
    color: "lightgrey"

    signal requestEncoders(codec: string)
    signal requestVideoFormats(codec: string)
    signal save(data: string, type: string)

    property variant formatsInfo: {
        'mp4': "The most common video format",
        'mkv': "The most common video format",
        'avi': "The most common video format",
        'mov': "The most common video format"
    }

    property variant codecsInfo: {
        'h264': {
            'description': "The most common codec",
            'encoders': {
                'libx264': {
                    'description': 'Open source most common software encoder',
                    'hardware': false
                },
                'libx264rgb': {
                    'description': 'Like libx264 but outputs RGB instead of YUV',
                    'hardware': false
                },
                'h264_amf': {
                    'description': "Utilizes AMD's AMF (Advanced Media Framework)",
                    'hardware': true
                },
                'h264_mf': {
                    'description': "Uses Microsoft's Media Foundation",
                    'hardware': true
                },
                'h264_nvenc': {
                    'description': "Utilizes NVIDIA's NVENC (NVIDIA Video Encoder) API",
                    'hardware': true
                },
                'h264_qsv': {
                    'description': "Uses Intel's Quick Sync Video (QSV)",
                    'hardware': true
                }
            }
        },
        'hevc': {
            'description': "The most efficient and modern codec",
            'encoders': {
                'libx265': {
                    'description': 'Open source most common software encoder',
                    'hardware': false
                },
                'hevc_amf': {
                    'description': "Utilizes AMD's AMF (Advanced Media Framework)",
                    'hardware': true
                },
                'hevc_mf': {
                    'description': "Uses Microsoft's Media Foundation",
                    'hardware': true
                },
                'hevc_nvenc': {
                    'description': "Utilizes NVIDIA's NVENC (NVIDIA Video Encoder) API",
                    'hardware': true
                },
                'hevc_qsv': {
                    'description': "Uses Intel's Quick Sync Video (QSV)",
                    'hardware': true
                }
            }
        },
        'mpeg4': {
            'description': "An older obsolete codec",
            'encoders': {
                'mpeg4': {
                    'description': 'Uses ffmpeg internal encoder implementation',
                    'hardware': false
                },
                'libxvid': {
                    'description': 'Uses the Xvid library',
                    'hardware': false
                }
            }
        },
        'prores': {
            'description': "Popular codec for editing",
            'encoders': {
                'libx264': {
                    'description': 'Most common software encoder',
                    'hardware': false
                }
            }
        }
    }


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
