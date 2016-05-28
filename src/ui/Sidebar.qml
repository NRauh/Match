import QtQuick 2.0
import QtQuick.Controls 1.2

Rectangle {
    id: sidebar
    width: 250
    height: 720
    color: "#c3d9e6"
    property var accountData
    property var offBudgetData
    property var targetLoader
    property var activeFile

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

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered: parent.color = "#99b1bf"
            onExited: parent.color = "#adc9d9"
            onClicked: targetLoader.setSource("BudgetView.qml", {activeFile: activeFile})
        }
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

    Column {
        id: accountList
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
                    onClicked: targetLoader.setSource("AccountView.qml", {
                                                          activeFile: activeFile,
                                                          accountId: modelData.accountId,
                                                          accountName: accountNameLabel.text
                                                      })
                }
            }
        }
    }

    Label {
        id: offBudgetLabel
        x: -7
        y: -2
        text: qsTr("Off Budget")
        anchors.leftMargin: 10
        font.pointSize: 18
        anchors.left: parent.left
        anchors.top: accountList.bottom
        z: 1
        anchors.topMargin: 15
    }

    Label {
        id: offBudgetBalances
        x: 192
        y: 7
        text: offBudgetData["balance"] || ""
        font.pointSize: 12
        anchors.top: accountList.bottom
        horizontalAlignment: Text.AlignRight
        anchors.topMargin: 25
        anchors.rightMargin: 10
        anchors.right: parent.right
    }

    Column {
        id: offBudgetList
        x: -8
        y: -2
        anchors.leftMargin: 0
        anchors.left: parent.left
        anchors.top: offBudgetLabel.bottom
        spacing: 8
        anchors.rightMargin: 0
        anchors.topMargin: 6
        anchors.right: parent.right
        Repeater {
            id: offBudgetItems
            model: offBudgetData["accounts"]
            Rectangle {
                id: offBudgetRect
                width: 250
                height: 25
                color: "#adc9d9"
                anchors.left: parent.left
                anchors.right: parent.right
                Label {
                    id: offBudgetAccountNameLabel
                    text: modelData.accountName || ""
                    anchors.leftMargin: 15
                    font.pointSize: 14
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                }

                Label {
                    id: offBudgetAccountBalanceLabel
                    text: modelData.accountBalance || ""
                    font.pointSize: 14
                    anchors.rightMargin: 10
                    anchors.right: parent.right
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: parent.color = "#99b1bf"
                    onExited: parent.color = "#adc9d9"
                    onClicked: targetLoader.setSource("AccountView.qml", {
                                                          activeFile: activeFile,
                                                          accountId: modelData.accountId,
                                                          accountName: offBudgetAccountNameLabel.text
                                                      })

                }
            }
        }
    }
}
