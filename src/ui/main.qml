import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2

ApplicationWindow {
    title: qsTr("Match")
    width: 1010
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
                        var win = component.createObject(matchWindow, {updateList: mainSidebar})
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
        id: mainSidebar
        width: 300
        height: 500
        targetLoader: contentLoader
    }

    Loader {
        id: contentLoader
        anchors.left: parent.left
        anchors.leftMargin: 300
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
    }
}
