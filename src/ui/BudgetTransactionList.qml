import QtQuick 2.0
import QtQuick.Controls 1.2
import com.nrauh 1.0

Item {
    id: budgetTransactionList
    width: 770
    height: 720
    property var budgetName
    property var monthInt
    property var month
    property var starting
    property var remaining
    property var transactions
    property var activeFile
    Component.onCompleted: {
        transactions = budget.getCategoryTransactions(activeFile, budgetName, monthInt);
    }

    Budget {
        id: budget
    }

    Rectangle {
        id: budgetInfoBackground
        height: 70
        color: "#e1e7ed"
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0

        Label {
            id: categoryName
            text: budgetName
            anchors.left: parent.left
            anchors.leftMargin: 8
            anchors.top: parent.top
            anchors.topMargin: 8
            font.pointSize: 16
        }

        Label {
            id: selectedMonth
            y: 43
            color: "#767676"
            text: month
            anchors.left: parent.left
            anchors.leftMargin: 8
            font.pointSize: 9
        }

        Label {
            id: startingAmount
            x: 686
            text: starting
            anchors.top: parent.top
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 8
            horizontalAlignment: Text.AlignRight
            font.pointSize: 16
        }

        Label {
            id: startingAmountLabel
            x: 664
            y: 43
            color: "#767676"
            text: qsTr("Starting Amount")
            anchors.right: parent.right
            anchors.rightMargin: 8
        }

        Label {
            id: remainingAmount
            x: 314
            text: remaining
            anchors.top: parent.top
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: 16
        }

        Label {
            id: remainingAmountLabel
            x: 328
            y: 43
            color: "#767676"
            text: qsTr("Remaining Amount")
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    TableView {
        id: transactionListTable
        anchors.top: parent.top
        anchors.topMargin: 70
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        frameVisible: false
        model: transactions
        TableViewColumn {
            role: "date"
            title: "Date"
            width: 80
        }
        TableViewColumn {
            role: "payee"
            title: "Payee"
            width: 150
        }
        TableViewColumn {
            role: "amount"
            title: "Amount"
            width: 70
        }
        TableViewColumn {
            role: "account"
            title: "Account"
            width: 150
        }
        TableViewColumn {
            role: "note"
            title: "Note"
            width: 270
        }
    }

}

