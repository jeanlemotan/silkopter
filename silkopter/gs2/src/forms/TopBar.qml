import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Rectangle  {
    property string title
    property alias trayArea: trayArea
    signal backPressed()

    id: root

    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    height: titleText.height + 16
    color: "#34495e"
    Text {
        id: titleText
        color: "#7f8c8d"
        text: title
        x: 8
        anchors.verticalCenter: parent.verticalCenter
        font.pointSize: 20; font.bold: true
    }

    Rectangle {
        id: trayArea
        anchors.left: titleText.right
        anchors.right: backButton.left
        anchors.leftMargin: 10
        anchors.rightMargin: 10
    }

    TrayButton {
        id: backButton
        //color: "#bdc3c7"
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 4
        width: height
        icon: "qrc:/icons/ui/back.png"
        onClicked: root.backPressed()
    }
}
