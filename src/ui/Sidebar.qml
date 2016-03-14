import QtQuick 2.0
import QtQuick.Controls 1.2
import com.nrauh 1.0

Rectangle {
    id: sidebar
    width: 250
    height: 720
    color: "#c3d9e6"
    property var accountData
    property var targetLoader
    Component.onCompleted: {
        var filePath = accManager.getLastFile()
        var accountList = account.getAccountListString(filePath)
        accountData = JSON.parse(accountList)
    }

    Label {
        id: logo
        width: 0
        height: 47
        text: qsTr("Match")
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 67
        anchors.left: parent.left
        anchors.leftMargin: 67
        font.family: "Arial"
        font.pointSize: 32
    }

    AccountManager {
        id: accManager
    }

    Rectangle {
        id: budgetBackground
        height: 60
        color: "#adc9d9"
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 145

        Label {
            id: budgetLabel
            text: qsTr("Budget")
            anchors.top: parent.top
            anchors.topMargin: 11
            anchors.left: parent.left
            anchors.leftMargin: 10
            font.pointSize: 22
        }
    }

    Label {
        id: accountBalances
        x: 199
        text: accountData["balance"] || ""
        horizontalAlignment: Text.AlignRight
        font.pointSize: 12
        anchors.top: parent.top
        anchors.topMargin: 226
        anchors.right: parent.right
        anchors.rightMargin: 10
    }

    Account {
        id: account
    }


    Label {
        id: accountsLabel
        text: qsTr("Accounts")
        z: 1
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 220
        font.pointSize: 18
    }

    Column {
        id: accountList
        height: 211
        spacing: 8
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 259
        Repeater {
            id: accountItems
            model: accountData["accounts"]
            Rectangle {
                id: accountRect
                width: 250
                height: 25
                anchors.right: parent.right
                anchors.left: parent.left
                color: "#adc9d9"

                Label {
                    id: accountNameLabel
                    text: modelData.accountName || ""
                    font.pointSize: 14
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 15
                }

                Label {
                    id: accountBalanceLabel
                    text: modelData.accountBalance || ""
                    font.pointSize: 14
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: parent.color = "#99b1bf"
                    onExited: parent.color = "#adc9d9"
                    onClicked: targetLoader.setSource("AccountView.qml", {accountIndex: modelData.index,
                                                          accountName: accountNameLabel.text})
                }
            }
        }
    }



}
