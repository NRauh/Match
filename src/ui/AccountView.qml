import QtQuick 2.0
import QtQuick.Controls 1.2

Rectangle {
    id: rectangle1
    width: 710
    height: 500
    color: "#ffffff"
    property var accountIndex
    property var accountName


    Rectangle {
        id: titleBackground
        height: 42
        color: "#b6bac5"
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0

        Label {
            id: accountNameLabel
            x: 20
            y: 10
            text: accountName
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 10
            font.pointSize: 18
        }

        Label {
            id: accountBalanceLabel
            x: 315
            y: 15
            height: 22
            text: "Balance"
            anchors.right: parent.right
            anchors.rightMargin: 20
            font.pointSize: 18
            anchors.topMargin: 10
            anchors.top: parent.top
        }
    }

    TableView {
        id: transactionTable
        anchors.top: parent.top
        anchors.topMargin: 132
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        headerVisible: true
        frameVisible: false
        TableViewColumn {
            role: "transactionDate"
            title: "Date"
            width: 80
        }
        TableViewColumn {
            role: "payee"
            title: "Payee"
            width: 125
        }
        TableViewColumn {
            role: "note"
            title: "Note"
            width: 200
        }
        TableViewColumn {
            role: "outflow"
            title: "Expense"
            width: 90
        }
        TableViewColumn {
            role: "income"
            title: "Income"
            width: 90
        }
        TableViewColumn {
            role: "category"
            title: "Category"
            width: 125
        }
        model: [{
                transactionDate: "12/18/2015",
                payee: "The Barber Shop",
                note: "Fancy new haircut and tip",
                outflow: "$25.00",
                income: "",
                category: "Health & Beauty"
            }]
    }

    Rectangle {
        id: inputBackground
        color: "#e1e6d9"
        anchors.top: parent.top
        anchors.topMargin: 42
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 368
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0

        Label {
            id: dateLabel
            x: 8
            y: 8
            text: qsTr("Date")
            font.pointSize: 12
        }

        Label {
            id: payeeLabel
            x: 81
            y: 8
            text: qsTr("Payee")
            font.pointSize: 12
        }

        Label {
            id: noteLabel
            x: 205
            y: 8
            text: qsTr("Note")
            font.pointSize: 12
        }

        Label {
            id: expenseLabel
            x: 405
            y: 8
            text: qsTr("Expense")
            font.pointSize: 12
        }

        Label {
            id: incomeLabel
            x: 495
            y: 10
            text: qsTr("Income")
            font.pointSize: 12
        }

        Label {
            id: categoryLabel
            x: 585
            y: 8
            text: qsTr("Category")
            font.pointSize: 12
        }

        TextField {
            id: dateInput
            x: 8
            y: 29
            width: 73
            height: 22
            placeholderText: qsTr("MM/DD/YYYY")
            validator: RegExpValidator { regExp: /\d\d\/\d\d\/\d\d\d\d/ }
        }

        TextField {
            id: payeeInput
            x: 87
            y: 29
            width: 109
            height: 22
            clip: false
            placeholderText: qsTr("Payee")
        }

        TextField {
            id: noteInput
            x: 205
            y: 29
            width: 190
            placeholderText: qsTr("Note")
        }

        TextField {
            id: expenseInput
            x: 405
            y: 30
            width: 80
            placeholderText: qsTr("0.00")
            validator: RegExpValidator { regExp: /\d+\.\d\d/ }
        }

        TextField {
            id: incomeInput
            x: 495
            y: 29
            width: 80
            placeholderText: qsTr("0.00")
            validator: RegExpValidator { regExp: /\d+\.\d\d/ }
        }

        ComboBox {
            id: categoryInput
            x: 585
            y: 29
            width: 117
            height: 23
        }

        Button {
            id: addTransactionButton
            x: 568
            y: 57
            text: qsTr("Add Transaction")
        }



        Button {
            id: cancelTransactionButton
            x: 485
            y: 57
            text: qsTr("Cancel")
            onClicked: {
                dateInput.text = ""
                payeeInput.text = ""
                noteInput.text = ""
                expenseInput.text = ""
                incomeInput.text = ""
            }
        }
    }
}

