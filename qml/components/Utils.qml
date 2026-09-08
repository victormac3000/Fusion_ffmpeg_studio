import QtQuick 2.15

import FusionFFmpegStudio

Item {
    id: root

    function wait(milliseconds, callback) {
        var timer = Qt.createQmlObject(
            'import QtQuick; Timer { interval: ' + milliseconds + '; repeat: false }',
            root
        )

        timer.triggered.connect(function() {
            callback()
            timer.destroy()
        })

        timer.start()
    }
}
