import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import com.nrauh 1.0

Window {
    id: newCheckingWindow
    width: 600
    height: 500
    visible: true

    Label {
        id: newCheckingLabel
        width: 232
        height: 50
        text: qsTr("New Checking Account")
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.top: parent.top
        anchors.topMargin: 10
        font.family: "Arial"
        font.pointSize: 40
    }

    Label {
        id: nameLabel
        text: qsTr("Checking Account Name")
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.top: parent.top
        anchors.topMargin: 139
        font.family: "Arial"
        font.pointSize: 22
    }

    TextField {
        id: accountName
        height: 27
        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.top: parent.top
        anchors.topMargin: 170
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
        anchors.topMargin: 223
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
            account.addChecking(filePath, accountName.text, accountBalanceInt, accountBalanceDate.selectedDate)
            newCheckingWindow.close()
        }
    }

    Button {
        id: cancelButton
        x: 424
        y: 464
        text: qsTr("Cancel")
        anchors.right: parent.right
        anchors.rightMargin: 99
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        onClicked: newCheckingWindow.close()
    }

    TextField {
        id: accountBalance
        x: -2
        y: 9
        height: 27
        anchors.top: parent.top
        font.pointSize: 22
        anchors.rightMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 322
        anchors.topMargin: 223
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
        anchors.topMargin: 274
        font.family: "Arial"
    }

    Calendar {
        id: accountBalanceDate
        height: 167
        anchors.left: parent.left
        anchors.leftMargin: 322
        anchors.top: parent.top
        anchors.topMargin: 274
        anchors.right: parent.right
        anchors.rightMargin: 50
    }
}

