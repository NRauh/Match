import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import com.nrauh 1.0

Window {
    id: accountSettingsWindow
    width: 600
    height: 200
    property var accounts
    property var activeFile
    property var selectedAccount
    property var updateList
    Component.onCompleted: {
        activeFile = accManager.getLastFile()
        accounts = account.getAccountList(activeFile)
    }

    Account {
        id: account
    }
    AccountManager {
        id: accManager
    }

    TableView {
        id: accountListTable
        frameVisible: false
        anchors.right: parent.right
        anchors.rightMargin: 150
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        model: accounts["accounts"]
        TableViewColumn {
            role: "accountName"
            title: "Account Name"
        }
        onClicked: {
            selectedAccount = model[row].accountId
            onBudget.enabled = true
            onBudget.checked = account.isOnBudget(activeFile, selectedAccount)
            deleteAccount.enabled = true
        }
    }

    CheckBox {
        id: onBudget
        x: 497
        text: "On Budget"
        enabled: false
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        onCheckedChanged: account.changeOnBudget(activeFile, selectedAccount, onBudget.checked)
    }

    Button {
        id: deleteAccount
        x: 475
        text: qsTr("Delete Account")
        enabled: false
        anchors.top: onBudget.bottom
        anchors.topMargin: 30
        anchors.right: parent.right
        anchors.rightMargin: 10
        onClicked: confirmationDialog.visible = true
    }

    Dialog {
        id: confirmationDialog
        title: "Delete this account?"
        width: 500
        Label {
            text: "Are you sure you want to permanently delete this account and it's transactions?"
        }
        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: account.deleteAccount(activeFile, selectedAccount)
    }

    Button {
        id: doneButton
        x: 499
        y: 138
        text: qsTr("Done")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        onClicked: {
            updateList.accountData = account.getAccountList(activeFile, 1)
            updateList.offBudgetData = account.getAccountList(activeFile, 2)
            accountSettingsWindow.close();
        }
    }
}
