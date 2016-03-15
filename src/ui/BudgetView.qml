import QtQuick 2.0
import QtQuick.Controls 1.4

Item {
    id: item1
    width: 770
    height: 720

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
            text: "March, 2016"
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 16
        }

        Image {
            id: leftArrow
            width: 25
            height: 58
            anchors.top: parent.top
            anchors.topMargin: 50
            anchors.leftMargin: 10
            anchors.horizontalCenter: item1.horizontalCenter
            anchors.right: item1.left
            anchors.left: parent.left
            source: "qrc:/qtquickplugin/images/template_image.png"
        }

        Label {
            id: previousMonthLabel
            x: 41
            y: 50
            width: 53
            height: 35
            text: qsTr("Previous Month")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            wrapMode: Text.WordWrap
        }

        Label {
            id: previousMonthBudget
            x: 41
            y: 91
            text: qsTr("(444.32)")
            horizontalAlignment: Text.AlignLeft
        }

        Image {
            id: rightArrow
            x: 165
            width: 25
            height: 58
            anchors.top: parent.top
            anchors.topMargin: 50
            anchors.right: parent.right
            anchors.rightMargin: 10
            source: "qrc:/qtquickplugin/images/template_image.png"
            anchors.horizontalCenter: item1.horizontalCenter
        }

        Label {
            id: nextMonthLabel
            x: 113
            y: 50
            width: 46
            height: 35
            text: qsTr("Next Month")
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }

        Label {
            id: nextMonthBudget
            x: 116
            y: 91
            text: qsTr("(444.32)")
            horizontalAlignment: Text.AlignRight
        }


        Label {
            id: remainingBudget
            height: 33
            text: qsTr("220.35")
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
            text: qsTr("500.00")
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

        Button {
            id: newCategoryButton
            y: 685
            text: qsTr("New Category")
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
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
            placeholderText: qsTr("Amount")
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
            id: categoryNameLabel
            y: 555
            text: qsTr("Category Name")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 148
            anchors.left: parent.left
            anchors.leftMargin: 10
        }

        ComboBox {
            id: parentCategoryInput
            y: 506
            height: 33
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 181
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
        }

        Label {
            id: parentCategoryLabel
            y: 483
            text: qsTr("Parent Category")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 220
            anchors.left: parent.left
            anchors.leftMargin: 10
        }
    }

    Rectangle {
        id: parentCategoryBackground
        x: 0
        y: 0
        width: 570
        height: 40
        color: "#dadfe6"

        Image {
            id: chevron
            y: 8
            width: 24
            height: 24
            anchors.left: parent.left
            anchors.leftMargin: 8
            source: "qrc:/qtquickplugin/images/template_image.png"
        }

        Text {
            id: parentLabel
            y: 10
            text: qsTr("Parent Category")
            anchors.left: parent.left
            anchors.leftMargin: 38
            font.pixelSize: 15
        }

        Image {
            id: parentCategorySearch
            x: 538
            y: 8
            width: 24
            height: 24
            anchors.right: parent.right
            anchors.rightMargin: 8
            source: "qrc:/qtquickplugin/images/template_image.png"
        }

        Text {
            id: parentRemainingBudget
            x: 478
            y: 10
            text: qsTr("100.00")
            anchors.right: parent.right
            anchors.rightMargin: 38
            horizontalAlignment: Text.AlignRight
            font.pixelSize: 15
        }
    }

    Rectangle {
        id: categoryBackground
        x: 0
        y: 40
        width: 570
        height: 35
        color: "#ffffff"
        border.width: 2
        border.color: "#dadfe6"

        Text {
            id: categoryLabel
            y: 8
            text: qsTr("Category")
            anchors.left: parent.left
            anchors.leftMargin: 48
            font.pixelSize: 14
        }

        Image {
            id: categorySearch
            x: 543
            y: 8
            width: 19
            height: 19
            anchors.right: parent.right
            anchors.rightMargin: 8
            source: "qrc:/qtquickplugin/images/template_image.png"
        }

        Text {
            id: categoryRemainingBudget
            x: 495
            y: 8
            text: qsTr("100.00")
            anchors.right: parent.right
            anchors.rightMargin: 33
            horizontalAlignment: Text.AlignRight
            font.pixelSize: 14
        }
    }

}

