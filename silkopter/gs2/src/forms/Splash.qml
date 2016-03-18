import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

Rectangle {
    id: root
    width: 800; height: 480
    color: "#2c3e50"

    signal startPressed()
    signal poweroffPressed()

    Text {
        id: title
        color: "#ecf0f1"
        text: "Silkopter"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 42; font.bold: true
    }

    MenuButton  {
        id: startButton
        anchors.horizontalCenter: parent.horizontalCenter
        y: title.y + 100
        width: 100
        height: width
        icon: "qrc:/icons/ui/fly.png"

        SequentialAnimation on color {
            loops: Animation.Infinite
            ColorAnimation { from: "#3498db"; to: "#7f8c8d"; duration: 500 }
            ColorAnimation { from: "#7f8c8d"; to: "#3498db"; duration: 500 }
        }

        onClicked: root.startPressed()
    }

    Dialog {
        id: poweroffDialog
        visible: false
        title: "Shutdown"
        onYes: root.poweroffPressed()
        standardButtons: StandardButton.Yes | StandardButton.No
        width: 400
        height: 100

        Text {
            text: "Are you sure you want to shutdown the system?"
            anchors.centerIn: parent
            height: 30
//            anchors.margins: 16
        }
    }

    MenuButton  {
        id: poweroffButton
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: 32
        height: width
        icon: "qrc:/icons/ui/reconnect.png"
        anchors.margins: 8
        color: "#C5EFF7"

        onClicked: poweroffDialog.open()
    }
}
