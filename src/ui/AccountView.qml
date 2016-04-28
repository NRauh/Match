import QtQuick 2.0
import QtQuick.Controls 1.4
import com.nrauh 1.0

Rectangle {
    id: background
    width: 770
    height: 720
    color: "#ffffff"
    property var accountId
    property var accountName
    property var transactions
    property var selectedTransactionId
    property var categories
    Component.onCompleted: {
        var lastFile = accManager.getLastFile()
        transactions = account.getTransactions(lastFile, accountId)
        // TODO: fix this returning a weird not quite array
        categories = budget.getCategoryNames(lastFile);
        categories = categories.toString();
        categories = categories.split(",");
    }

    AccountManager {
        id: accManager
    }
    Account {
        id: account
    }
    Budget {
        id: budget
    }


    Rectangle {
        id: actionBar
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
            validator: RegExpValidator { regExp: /\d+\.\d\d/ }
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
            model: categories
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
            onClicked: {
                var lastFile = accManager.getLastFile();
                var acceptable = true

                if (!payeeInput.text) {
                    acceptable = false
                }
                if (!noteInput.text) {
                    acceptable = false
                }

                if (acceptable) {
                    account.addTransaction(lastFile, accountId,
                                           dateInput.selectedDate,
                                           payeeInput.text,
                                           outflowInput.checked,
                                           parseFloat(amountInput.text) * 100,
                                           categoryInput.currentText,
                                           noteInput.text)
                    payeeInput.text = ""
                    outflowInput.checked = true
                    amountInput.text = ""
                    noteInput.text = ""
                    // TODO: Should arrange by date
                    transactions = account.getTransactions(lastFile, accountId);
                }
            }
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
            onClicked: {
                payeeInput.text = ""
                outflowInput.checked = true
                amountInput.text = ""
                noteInput.text = ""
            }
        }

        Button {
            id: updateTransactionButton
            x: -5
            y: 7
            height: 27
            text: qsTr("Update Transaction")
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.rightMargin: 54
            anchors.topMargin: 647
            anchors.leftMargin: 10
            anchors.left: parent.left
            visible: false
            onClicked: {
                var lastFile = accManager.getLastFile();
                account.updateTransaction(lastFile, selectedTransactionId,
                                          dateInput.selectedDate,
                                          payeeInput.text,
                                          outflowInput.checked,
                                          parseFloat(amountInput.text) * 100,
                                          noteInput.text)

                updateTransactionButton.visible = false
                doneButton.visible = false
                deleteButton.visible = false
                addTransactionButton.visible = true
                resetButton.visible = true
                payeeInput.text = ""
                noteInput.text = ""
                amountInput.text = ""
                outflowInput.checked = true

                transactions = account.getTransactions(lastFile, accountId);
            }
        }

        Button {
            id: doneButton
            x: -2
            y: -6
            text: qsTr("Done")
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.rightMargin: 105
            anchors.topMargin: 680
            anchors.leftMargin: 10
            anchors.left: parent.left
            visible: false
            onClicked: {
                updateTransactionButton.visible = false
                doneButton.visible = false
                deleteButton.visible = false
                addTransactionButton.visible = true
                resetButton.visible = true
                payeeInput.text = ""
                noteInput.text = ""
                amountInput.text = ""
                outflowInput.checked = true
            }
        }

        Button {
            id: deleteButton
            x: 2
            y: -2
            text: qsTr("Delete")
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.topMargin: 680
            anchors.leftMargin: 105
            anchors.left: parent.left
            visible: false
            onClicked: {
                var lastFile = accManager.getLastFile()
                account.deleteTransaction(lastFile, selectedTransactionId)

                updateTransactionButton.visible = false
                doneButton.visible = false
                deleteButton.visible = false
                addTransactionButton.visible = true
                resetButton.visible = true
                payeeInput.text = ""
                noteInput.text = ""
                amountInput.text = ""
                outflowInput.checked = true

                transactions = account.getTransactions(lastFile, accountId)
            }
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
            role: "category"
            title: "Category"
            width: 120
        }
        model: transactions["transactions"]
        onClicked: {
            updateTransactionButton.visible = true
            doneButton.visible = true
            deleteButton.visible = true
            addTransactionButton.visible = false
            resetButton.visible = false

            var transactionDate = new Date(model[row].intDate)
            transactionDate = new Date(transactionDate.getTime() + transactionDate.getTimezoneOffset() * 60000)
            dateInput.selectedDate = transactionDate
            payeeInput.text = model[row].payee
            noteInput.text = model[row].note
            amountInput.text = parseFloat(model[row].amount.substr(1)).toFixed(2)
            outflowInput.checked = model[row].outflow
            selectedTransactionId = model[row].id;
        }
    }
}

