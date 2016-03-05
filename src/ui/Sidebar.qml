import QtQuick 2.0
import QtQuick.Controls 1.2

Rectangle {
    id: sidebarr
    width: 300
    height: 500
    color: "#e5e7e8"
    property var accountData

    Label {
        id: logo
        height: 50
        text: qsTr("Match")
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.right: parent.right
        anchors.rightMargin: 93
        anchors.left: parent.left
        anchors.leftMargin: 94
        font.family: "Arial"
        font.pointSize: 42
    }

    Label {
        id: budgetLabel
        text: qsTr("Budget")
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 114
        font.pointSize: 18
    }

    Label {
        id: accountsLabel
        text: qsTr("Accounts")
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 152
        font.pointSize: 18
    }

    Column {
        id: accountList
        height: 211
        anchors.right: parent.right
        anchors.rightMargin: 28
        anchors.left: parent.left
        anchors.leftMargin: 28
        anchors.top: parent.top
        anchors.topMargin: 180
        Repeater {
            id: accountItems
            model: accountData["accounts"]
            Rectangle {
                id: accountRect
                anchors.right: parent.right
                anchors.left: parent.left
                height: 30
                color: "#cdd3d6"
                border.width: 1

                Label {
                    id: accountNameLabel
                    text: modelData.accountName
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 5
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: parent.color = "#e5e7e8"
                    onExited: parent.color = "#cdd3d6"
                }
            }
        }
    }

}
