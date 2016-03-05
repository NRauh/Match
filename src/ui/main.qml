import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2

ApplicationWindow {
    title: qsTr("Match")
    width: 800
    height: 500
    visible: true
    id: matchWindow

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Open")
            }
            Menu {
                title: qsTr("&New Account")
                MenuItem {
                    text: qsTr("&Checking")
                    onTriggered: {
                        var component = Qt.createComponent("newchecking.qml")
                        var win = component.createObject(matchWindow)
                        win.show()
                    }
                }
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

    Sidebar {
        accountData: [{accountName: "foo", accountType: "Checking"}]
    }
}
