import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

ComboBox {
    signal save(data: string, type: string)
    signal addCompleted(currentText: string)

    property string actualText: ""

    model: ListModel {}
    currentIndex: 0

    function add(list, defaultValue) {
        model.clear()
        var foundDefault = false
        for (var i=0; i<list.length; i++) {
            model.append({"text": list[i]})
            if (list[i] === defaultValue) {
                foundDefault = true
                currentIndex = i
            }
        }
        if (!foundDefault) {
            currentIndex = 0
        }
        if (list.length > 0) {
            addCompleted(model.get(currentIndex).text)
        }
    }

}
