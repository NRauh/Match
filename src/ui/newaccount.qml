import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import com.nrauh 1.0

Window {
    id: newCheckingWindow
    width: 600
    height: 525
    visible: true
    //modality: Qt.WindowModal
    property var updateList

    Label {
        id: newCheckingLabel
        width: 545
        height: 63
        text: qsTr("New  Account")
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.top: parent.top
        anchors.topMargin: 10
        font.family: "Arial"
        font.pointSize: 40
    }

    Label {
        id: nameLabel
        text: qsTr("Account Name")
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.top: parent.top
        anchors.topMargin: 119
        font.family: "Arial"
        font.pointSize: 22
    }

    TextField {
        id: accountName
        height: 32
        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.top: parent.top
        anchors.topMargin: 159
        font.family: "Arial"
        font.pointSize: 22
        placeholderText: qsTr("Account Name")
    }

    Label {
        id: accountBalanceLabel
        x: 0
        y: 0
        text: qsTr("Account Balance")
        anchors.leftMargin: 50
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 207
        font.pointSize: 22
        font.family: "Arial"
    }

    Account {
        id: account
    }
    AccountManager {
        id: accManager
    }

    Button {
        id: finishButton
        x: 505
        y: 464
        text: qsTr("Finish")
        anchors.right: parent.right
        anchors.rightMargin: 25
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        onClicked: {
            var filePath = accManager.getLastFile()
            var accountBalanceInt = parseFloat(accountBalance.text) * 100
            var balanceDate = accountBalanceDate.selectedDate
            var onBudgetStatus = onBudget.checked
            account.addAccount(filePath, accountName.text, accountBalanceInt, balanceDate, onBudgetStatus)

            var accountList = account.getAccountList(filePath)
            updateList.accountData = accountList
            newCheckingWindow.close()
        }
    }

    Button {
        id: cancelButton
        x: 399
        y: 463
        text: qsTr("Cancel")
        anchors.right: parent.right
        anchors.rightMargin: 116
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        onClicked: newCheckingWindow.close()
    }

    TextField {
        id: accountBalance
        x: -2
        y: 9
        height: 32
        anchors.top: parent.top
        font.pointSize: 22
        anchors.rightMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 322
        anchors.topMargin: 207
        font.family: "Arial"
        anchors.right: parent.right
        placeholderText: qsTr("Balance")
        validator: RegExpValidator { regExp: /\d+\.\d\d/ }
    }

    Label {
        id: balanceDateLabel
        x: -5
        y: 6
        text: qsTr("Balance Date")
        anchors.top: parent.top
        font.pointSize: 22
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.topMargin: 257
        font.family: "Arial"
    }

    Calendar {
        id: accountBalanceDate
        height: 167
        anchors.left: parent.left
        anchors.leftMargin: 322
        anchors.top: parent.top
        anchors.topMargin: 257
        anchors.right: parent.right
        anchors.rightMargin: 50
    }

    CheckBox {
        id: onBudget
        x: 464
        text: qsTr("On Budget")
        checked: true
        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.top: parent.top
        anchors.topMargin: 440
    }
}

