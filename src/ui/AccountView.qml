import QtQuick 2.0
import QtQuick.Controls 1.2

Rectangle {
    width: 100
    height: 62
    property var accountIndex

    Label {
        id: foo
        x: 20
        y: 20
        text: accountIndex
    }
}

