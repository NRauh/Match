import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import com.nrauh 1.0

ApplicationWindow {
    title: qsTr("Match")
    width: 995
    height: 735
    visible: true
    id: matchWindow
    Component.onCompleted: {
        var lastFile = accManager.getLastFile()

        if (accManager.haveLastFile()) {
            initialize(lastFile);
        } else {
            var newBudget = Qt.createComponent("newbudget.qml")
            var win = newBudget.createObject(matchWindow, {reloadParent: initialize})
            win.show()
        }
   }

    function initialize(file) {
        var now = new Date()
        accManager.shiftBudget(file, now)
        var accountList = account.getAccountList(file, 1)
        var offBudgetList = account.getAccountList(file, 2)
        contentLoader.setSource("BudgetView.qml", {activeFile: file, targetLoader: contentLoader});
        mainSidebar.setSource("Sidebar.qml", {
                                  activeFile: file,
                                  targetLoader: contentLoader,
                                  accountData: accountList,
                                  offBudgetData: offBudgetList
                              })
    }

    AccountManager {
        id: accManager
    }

    Account {
        id: account
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&New Budget");
                onTriggered: {
                    var component = Qt.createComponent("newbudget.qml")
                    var win = component.createObject(matchWindow)
                    win.reloadParent = initialize
                    win.show()
                }
            }
            MenuItem {
                text: qsTr("&Open Budget")
                onTriggered: {
                    openDialog.open()
                }
            }
           MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
        Menu {
            title: qsTr("&Accounts")
            MenuItem {
               text: qsTr("&New Account")
               onTriggered: {
                   var component = Qt.createComponent("newaccount.qml")
                   var win = component.createObject(matchWindow, {updateList: mainSidebar.item})
                   win.show()
               }
           }
            MenuItem {
                text: qsTr("&Account Settings")
                onTriggered: {
                    var component = Qt.createComponent("AccountSettings.qml")
                    var accSettings = component.createObject(matchWindow, {updateList: mainSidebar.item})
                    accSettings.show()
                }
            }
        }
    }

    FileDialog {
        id: openDialog
        title: "Select budget file to open"
        folder: shortcuts.documents
        nameFilters: ["Match budget file (*.mbgt)"]
        onAccepted: {
            accManager.setLastFile(openDialog.fileUrl)
            initialize(openDialog.fileUrl)
        }
    }

    Loader {
        id: mainSidebar
        width: 225
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
    }

    Loader {
        id: contentLoader
        anchors.left: parent.left
        anchors.leftMargin: 225
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
    }
}
