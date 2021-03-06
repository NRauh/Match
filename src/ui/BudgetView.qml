import QtQuick 2.0
import QtQuick.Controls 1.4
import com.nrauh 1.0

Item {
    id: budgetView
    width: 770
    height: 720
    property var categories
    property var activeFile
    property var selectedMonth
    property var meta
    property var available
    property var targetLoader
    Component.onCompleted: {
        selectedMonth = 0;
        categories = budget.getCategories(activeFile, selectedMonth);
        meta = budget.getMeta(activeFile, selectedMonth);
        available = budget.getAvailableMoney(activeFile);
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
            id: monthLabel
            height: 27
            text: meta["month"]
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 16
        }

        Rectangle {
            id: previousMonth
            width: 100
            height: 60
            color: "#00000000"
            anchors.top: parent.top
            anchors.topMargin: 50
            Image {
                id: leftArrow
                width: 25
                height: 58
                anchors.top: parent.top
                anchors.topMargin: 1
                anchors.left: parent.left
                anchors.leftMargin: 10
                source: "../assets/leftarrow.png"

            }

            Label {
                id: previousMonthLabel
                x: 39
                y: 13
                width: 53
                height: 35
                text: qsTr("Previous Month")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                wrapMode: Text.WordWrap
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: parent.color = "#c8ced4"
                onExited: parent.color = "#00000000"
                onClicked: {
                    selectedMonth = selectedMonth - 1
                    categories = budget.getCategories(activeFile, selectedMonth)
                    meta = budget.getMeta(activeFile, selectedMonth);
                    if (selectedMonth === -2) {
                        previousMonth.visible = false
                    }
                    if (selectedMonth < 2) {
                        nextMonth.visible = true
                    }
                }
            }
        }

        Rectangle {
            id: nextMonth
            x: 100
            width: 100
            height: 60
            color: "#00000000"
            anchors.top: parent.top
            anchors.topMargin: 50
            Image {
                id: rightArrow
                width: 25
                height: 58
                anchors.top: parent.top
                anchors.topMargin: 1
                anchors.right: parent.right
                anchors.rightMargin: 10
                source: "../assets/rightarrow.png"
            }

            Label {
                id: nextMonthLabel
                x: 15
                y: 13
                width: 46
                height: 35
                text: qsTr("Next Month")
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: parent.color = "#c8ced4"
                onExited: parent.color = "#00000000"
                onClicked: {
                    selectedMonth = selectedMonth + 1
                    categories = budget.getCategories(activeFile, selectedMonth)
                    meta = budget.getMeta(activeFile, selectedMonth)
                    if (selectedMonth === 2) {
                        nextMonth.visible = false
                    }
                    if (selectedMonth > -2) {
                        previousMonth.visible = true
                    }
                }
            }
        }

        Label {
            id: remainingBudget
            height: 33
            text: meta["remaining"]
            anchors.top: parent.top
            anchors.topMargin: 160
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            font.pointSize: 18
            horizontalAlignment: Text.AlignHCenter
        }

        Label {
            id: remainingBudgetLabel
            text: qsTr("Remaining Month")
            anchors.left: parent.left
            anchors.leftMargin: 48
            anchors.top: parent.top
            anchors.topMargin: 199
            anchors.right: parent.right
            anchors.rightMargin: 47
            font.pointSize: 9
        }

        Label {
            id: unbudgetAmount
            x: -8
            y: -7
            height: 26
            text: available
            font.pointSize: 14
            anchors.top: parent.top
            horizontalAlignment: Text.AlignHCenter
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            anchors.topMargin: 234
            anchors.right: parent.right
            anchors.left: parent.left
        }

        Label {
            id: unbudgetedAmountLabel
            text: qsTr("To Budget")
            anchors.top: parent.top
            anchors.topMargin: 266
            anchors.left: parent.left
            anchors.leftMargin: 70
            anchors.right: parent.right
            anchors.rightMargin: 69
        }

        Label {
            id: categoryNameLabel
            y: 555
            text: qsTr("Category Name")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 148
            anchors.left: parent.left
            anchors.leftMargin: 10
        }

        TextField {
            id: categoryNameInput
            y: 578
            height: 25
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 117
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            placeholderText: qsTr("Name")
        }

        Label {
            id: amountLabel
            y: 619
            text: qsTr("Amount")
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 84
        }

        TextField {
            id: amountInput
            y: 642
            height: 25
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 53
            placeholderText: qsTr("0.00")
            validator: RegExpValidator { regExp: /\d+\.\d\d/ }
        }

        Button {
            id: newCategoryButton
            y: 685
            text: qsTr("New Category")
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            onClicked: {
                var acceptable = true

                if (!categoryNameInput.text) {
                    acceptable = false
                }
                if (!amountInput.text) {
                    acceptable = false
                }

                if (acceptable) {
                    var categoryName = categoryNameInput.text
                    var amount = parseFloat(amountInput.text) * 100

                    budget.addCategory(activeFile, categoryName, amount);
                    categories = budget.getCategories(activeFile, 0);
                    meta = budget.getMeta(activeFile, selectedMonth)
                    available = budget.getAvailableMoney(activeFile)

                    categoryNameInput.text = ""
                    amountInput.text = ""
                }
            }
        }

        Button {
            id: updateCategoryButton
            y: 685
            text: qsTr("Update")
            visible: false
            anchors.left:  parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            onClicked: {
                var category = categoryNameInput.text
                var amount = parseFloat(amountInput.text) * 100

                budget.updateBudget(activeFile, selectedMonth, category, amount)
                categories = budget.getCategories(activeFile, selectedMonth)
                meta = budget.getMeta(activeFile, selectedMonth)
                available = budget.getAvailableMoney(activeFile)
            }
        }

        Button {
            id: cancelButton
            x: 136
            y: 683
            width: 54
            text: qsTr("Cancel")
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            onClicked: {
                categoryNameInput.text = "";
                amountInput.text = "";
                updateCategoryButton.visible = false
                newCategoryButton.visible = true
            }
        }
    }

    Column {
        id: categoryColumn
        anchors.right: parent.right
        anchors.rightMargin: 200
        anchors.left: parent.left
        anchors.leftMargin: 0
        Repeater {
            model: categories

            Rectangle {
                id: categoryBackground
                height: 40
                color: "#ffffff"
                anchors.left: categoryColumn.left
                anchors.leftMargin: 0
                anchors.right: categoryColumn.right
                anchors.rightMargin: 0
                border.width: 5
                border.color: "#dadfe6"

                Text {
                    id: categoryLabel
                    y: 10
                    text: modelData.categoryName
                    anchors.left: parent.left
                    anchors.leftMargin: 38
                    font.pixelSize: 15
                }

                Text {
                    id: categoryBudgetAmount
                    x: 478
                    y: 10
                    text: modelData.remaining
                    anchors.right: parent.right
                    anchors.rightMargin: 38
                    horizontalAlignment: text.AlignRight
                    font.pixelSize: 15
                }

                Image {
                    id: categorySearch
                    x: 538
                    y: 8
                    width: 24
                    height: 24
                    anchors.right: parent.right
                    anchors.rightMargin: 9
                    source: "../assets/search.png"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            targetLoader.setSource("BudgetTransactionList.qml", {
                                                       budgetName: modelData.categoryName,
                                                       month: meta["month"],
                                                       monthInt: meta["monthInt"],
                                                       starting: modelData.amount,
                                                       remaining: modelData.remaining,
                                                       targetLoader: targetLoader,
                                                       activeFile: activeFile
                                                   })
                        }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    anchors.rightMargin: 38
                    hoverEnabled: true
                    onEntered: parent.color = "#e6e6e6"
                    onExited: parent.color = "#ffffff"
                    onClicked: {
                        newCategoryButton.visible = false
                        updateCategoryButton.visible = true
                        categoryNameInput.text = modelData.categoryName
                        amountInput.text = modelData.amount
                    }
                }
            }
        }
    }
}

