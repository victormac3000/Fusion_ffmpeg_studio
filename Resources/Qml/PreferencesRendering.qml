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
        'mkv': "Open standard format",
        'avi': "Older propietary Microsoft format",
        'mov': "A format developed by Apple to store high resolution videos",
        "mxf": "A professional format mainly used for TV prroduction and broadcasting"
    }

    property variant codecsInfo: {
        'h264': {
            'description': "The most common codec\nUse libx265 for quality, anything else with hardware acceleration for speed",
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
            'description': "The most efficient and modern codec\nUse libx265 for quality, anything else with hardware acceleration for speed",
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
            'description': "An older obsolete codec\nUse libxvid unless you want more legacy suppprt. libxvid is faster and produces lighter and better quality videos",
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
            'description': "Popular codec for editing\nFor general use with a good balance between quality and performance: Use prores_aw.\nFor scenarios where output quality is paramount and you need fine control: Use prores_ks.\nFor basic encoding: Use prores",
            'encoders': {
                'prores': {
                    'description': 'Uses ffmpeg internal encoder implementation',
                    'hardware': false
                },
                'prores_aw': {
                    'description': 'Based on the libavcodec library’s implementation',
                    'hardware': false
                },
                'prores_ks': {
                    'description': 'Another implementation of the ProRes encoder in FFmpeg by Kostya Shishkov',
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
        encodersComboBox.currentIndexChanged()
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
        formatsComboBox.currentIndexChanged()
    }

    ColumnLayout {
        width: parent.width
        height: parent.height

        GridLayout {
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
            Layout.fillHeight: true
            Layout.fillWidth: true
            columns: 3

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Layout.maximumWidth: parent.width*0.15
                border.color: "green"
                border.width: 3

                Text {
                    anchors.fill: parent
                    font.pointSize: 12
                    minimumPointSize: 10
                    fontSizeMode: Text.Fit
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
                border.color: "green"
                border.width: 3

                TextEdit {
                    id: codecDescriptionText
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    clip: true
                    font.pointSize: 12
                    font.family: "Arial"
                    text: ""
                    wrapMode: Text.WordWrap
                    readOnly: true
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignTop
                    padding: 5
                }

            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                border.color: "green"
                border.width: 3

                ComboBox {
                    id: codecsComboBox
                    anchors.fill: parent
                    anchors.margins: 10
                    font.pointSize: 12
                    model: ListModel {}
                    currentIndex: 0

                    onCurrentIndexChanged: {
                        save(model.get(currentIndex).text, "codec")
                        var codecObject = codecsInfo[currentText]
                        if (typeof codecObject != "undefined") {
                            codecDescriptionText.text = codecObject.description
                        }
                        requestEncoders(model.get(currentIndex).text)
                        requestVideoFormats(model.get(currentIndex).text)
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
                    font.pointSize: 12
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
                Layout.alignment: Qt.AlignTop
                border.color: "green"
                border.width: 3

                TextEdit {
                    id: encodersDescriptionText
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    clip: true
                    font.pointSize: 12
                    font.family: "Arial"
                    text: ""
                    readOnly: true
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignTop
                    padding: 5
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                border.color: "green"
                border.width: 3

                ComboBox {
                    id: encodersComboBox
                    anchors.fill: parent
                    anchors.margins: 10
                    font.pointSize: 12
                    model: ListModel {}
                    currentIndex: 0

                    onCurrentIndexChanged: {
                        if (currentIndex >= 0) {
                            save(model.get(currentIndex).text, "encoder")
                            var codecObject = codecsInfo[codecsComboBox.currentText]
                            if (typeof codecObject != "undefined") {
                                var encoderObject = codecsInfo[codecsComboBox.currentText].encoders[currentText]
                                if (typeof encoderObject != "undefined") {
                                    encodersDescriptionText.text = encoderObject.description + "\n"
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
                    font.pointSize: 12
                    minimumPointSize: 10
                    fontSizeMode: Text.Fit
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
                border.color: "green"
                border.width: 3

                TextEdit {
                    id: videoFormatsDescriptionText
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    clip: true
                    font.pointSize: 12
                    font.family: "Arial"
                    text: ""
                    readOnly: true
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignTop
                    padding: 5
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                border.color: "green"
                border.width: 3

                ComboBox {
                    id: formatsComboBox
                    anchors.fill: parent
                    anchors.margins: 10
                    font.pointSize: 12
                    model: ListModel {}
                    currentIndex: 0

                    onCurrentIndexChanged: {
                        if (currentIndex >= 0) {
                            save(model.get(currentIndex).text, "format")
                            var videoFormatInfoObject = formatsInfo[currentText]
                            if (typeof videoFormatInfoObject != "undefined") {
                                videoFormatsDescriptionText.text = videoFormatInfoObject + "\n"
                            }
                        }
                    }
                }
            }
        }
    }
}
