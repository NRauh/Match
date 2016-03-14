import QtQuick 2.0
import QtQuick.Controls 1.4
import com.nrauh 1.0

Rectangle {
    id: rectangle1
    width: 770
    height: 720
    color: "#ffffff"
    property var accountIndex
    property var accountName
    property var transactions
    Component.onCompleted: {
        var lastFile = accManager.getLastFile()
        var transactionString = JSON.parse(account.getTransactionsString(lastFile, accountIndex))
        transactions = transactionString
    }

    AccountManager {
        id: accManager
    }
    Account {
        id: account
    }


    Rectangle {
        id: rectangle2
        x: 570
        width: 200
        color: "#e1e7ed"
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0

            Label {
            id: accountNameLabel
            height: 27
            text: accountName
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 16
        }

        Label {
            id: accountBalanceLabel
            y: 60
            height: 33
            text: transactions["balance"]
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            horizontalAlignment: Text.AlignHCenter
            anchors.top: parent.top
            anchors.topMargin: 65
            font.pointSize: 18
        }

        Label {
            id: dateLabel
            y: 140
            text: qsTr("Date")
            anchors.top: parent.top
            anchors.topMargin: 130
            anchors.right: parent.right
            anchors.rightMargin: 161
            anchors.left: parent.left
            anchors.leftMargin: 10
            font.pointSize: 9
        }

        Calendar {
            id: dateInput
            height: 187
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 153
            navigationBarVisible: true
            frameVisible: false
            dayOfWeekFormat: 1
            weekNumbersVisible: false
            visibleMonth: 2
        }

        Label {
            id: payeeLabel
            text: qsTr("Payee")
            anchors.right: parent.right
            anchors.rightMargin: 158
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 356
        }

        TextField {
            id: payeeInput
            height: 25
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 379
            placeholderText: qsTr("Payee")
        }

        Label {
            id: noteLabel
            text: qsTr("Note")
            anchors.right: parent.right
            anchors.rightMargin: 162
            anchors.top: parent.top
            anchors.topMargin: 420
            anchors.left: parent.left
            anchors.leftMargin: 10
        }

        TextArea {
            id: noteInput
            height: 60
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 443
            tabChangesFocus: true
        }

        Label {
            id: amountlabel
            text: qsTr("Amount")
            anchors.top: parent.top
            anchors.topMargin: 519
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 143
        }

        TextField {
            id: amountInput
            width: 90
            anchors.top: parent.top
            anchors.topMargin: 542
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 100
            placeholderText: qsTr("0.00")
        }

        CheckBox {
            id: outflowInput
            text: "Expense"
            anchors.top: parent.top
            anchors.topMargin: 544
            anchors.left: parent.left
            anchors.leftMargin: 115
            anchors.rightMargin: 10
            anchors.right: parent.right
            checked: true
        }

        Label {
            id: categoryLabel
            x: 10
            y: 583
            text: qsTr("Category")
        }

        ComboBox {
            id: categoryInput
            height: 25
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 606
        }

        Button {
            id: addTransactionButton
            text: qsTr("Add Transaction")
            anchors.right: parent.right
            anchors.rightMargin: 75
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 647
        }

        Button {
            id: resetButton
            text: qsTr("Reset")
            anchors.right: parent.right
            anchors.rightMargin: 105
            anchors.top: parent.top
            anchors.topMargin: 680
            anchors.left: parent.left
            anchors.leftMargin: 10
        }
    }

    TableView {
        id: transactionTable
        y: 0
        anchors.right: parent.right
        anchors.rightMargin: 200
        verticalScrollBarPolicy: 1
        horizontalScrollBarPolicy: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        headerVisible: true
        frameVisible: false
        TableViewColumn {
            role: "date"
            title: "Date"
            width: 80
        }
        TableViewColumn {
            role: "payee"
            title: "Payee"
            width: 100
        }
        TableViewColumn {
            role: "note"
            title: "Note"
            width: 165
        }
        TableViewColumn {
            role: "amount"
            title: "Amount"
            width: 70
        }
        TableViewColumn {
            role: "outflow"
            title: "Flow"
            width: 35
        }
        //TableViewColumn {
            //role: "category"
            //title: "Category"
            //width: 120
        //}
        model: transactions["transactions"]
    }
}

