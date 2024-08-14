import QtQuick 2.15

Item {

    property variant codecsInfo: {
        'h264': "The most common codec\nUse libx265 for quality, anything else with hardware acceleration for speed",
        'hevc': "The most efficient and modern codec\nUse libx265 for quality, anything else with hardware acceleration for speed",
        'mpeg4': "An older obsolete codec\nUse libxvid unless you want more legacy suppprt. libxvid is faster and produces lighter and better quality videos",
        'prores': "Popular codec for editing\nFor general use with a good balance between quality and performance: Use prores_aw.\nFor scenarios where output quality is paramount and you need fine control: Use prores_ks.\nFor basic encoding: Use prores"
    }

    property variant encodersInfo: {
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
        },
        'h264_videotoolbox': {
            'description': "Uses mac integrated video encoders",
            'hardware': true
        },
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
        },
        'hevc_videotoolbox': {
            'description': "Uses mac integrated video encoders",
            'hardware': true
        },
        'mpeg4': {
            'description': 'Uses ffmpeg internal encoder implementation',
            'hardware': false
        },
        'libxvid': {
            'description': 'Uses the Xvid library',
            'hardware': false
        },
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

    property variant formatsInfo: {
        'mp4': "The most common video format",
        'mkv': "Open standard format",
        'avi': "Older propietary Microsoft format",
        'mov': "A format developed by Apple to store high resolution videos",
        "mxf": "A professional format mainly used for TV prroduction and broadcasting"
    }
}

