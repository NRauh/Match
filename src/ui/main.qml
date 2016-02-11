import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2

ApplicationWindow {
    title: qsTr("Hello World")
    width: 640
    height: 480
    visible: true
    id: matchWindow

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Open")
            }
            MenuItem {
                text: qsTr("&New Budget");
                onTriggered: {
                    var component = Qt.createComponent("newbudget.qml")
                    var win = component.createObject(matchWindow)
                    win.show()
                }
            }
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }
}
