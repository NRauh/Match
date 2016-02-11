import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import com.nrauh 1.0

Window {
    id: window1
    width: 600
    height: 500

    Label {
        id: newBudgetLabel
        width: 232
        height: 50
        text: qsTr("New Budget")
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.top: parent.top
        anchors.topMargin: 10
        font.family: "Arial"
        font.pointSize: 40
    }

    Label {
        id: nameLabel
        text: qsTr("Give your budget a name")
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.top: parent.top
        anchors.topMargin: 139
        font.family: "Arial"
        font.pointSize: 22
    }

    TextField {
        id: budgetName
        height: 27
        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.top: parent.top
        anchors.topMargin: 170
        font.family: "Arial"
        font.pointSize: 22
        placeholderText: qsTr("Budget Name")
    }

    Label {
        id: pathLabel
        x: 0
        y: 0
        text: qsTr("Budget Save Location")
        anchors.leftMargin: 50
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 223
        font.pointSize: 22
        font.family: "Arial"
    }

    FileDialog {
        id: pathDialog
        title: "Where will your budget file live?"
        selectFolder: true
        onAccepted: pathLocation.text = pathDialog.fileUrl
        folder: "~"
    }

    Button {
        id: pathButton
        height: 27
        text: qsTr("Save Location")
        anchors.right: parent.right
        anchors.rightMargin: 440
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.top: parent.top
        anchors.topMargin: 254
        onClicked: {
            pathDialog.open()
        }
    }

    Label {
        id: pathLocation
        height: 19
        color: "#424242"
        text: qsTr("")
        anchors.left: parent.left
        anchors.leftMargin: 166
        anchors.top: parent.top
        anchors.topMargin: 258
        anchors.right: parent.right
        anchors.rightMargin: 50
        font.italic: true
        font.pointSize: 16
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
            accManager.createBudget(pathDialog.fileUrl, budgetName.text)
            //console.log(pathDialog.fileUrl, budgetName.text)
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
    }
}

