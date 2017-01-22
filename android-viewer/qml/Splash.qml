import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

Rectangle {
    id: root
    width: 800; height: 480
    color: "#2c3e50"

    Text {
        id: title
        color: "#ecf0f1"
        text: "Slkptr"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 42;
        font.bold: true
    }

    ToolButton
    {
        id: startButton
        anchors.top: title.bottom
        anchors.topMargin: 40
        anchors.horizontalCenter: parent.horizontalCenter
        width: 100
        height: width
        Image {
            source: "qrc:/icons/ui/fly.png"
            anchors.centerIn: parent
            anchors.margins: 8
            fillMode: Image.PreserveAspectFit
        }

        style: ButtonStyle {
            background: Rectangle {
                radius: 8
                SequentialAnimation on color {
                    loops: Animation.Infinite
                    ColorAnimation { from: "#6C7A89"; to: "#D35400"; duration: 500 }
                    ColorAnimation { from: "#D35400"; to: "#6C7A89"; duration: 500 }
                }
            }
        }
        onClicked: s_menus.push("MM.qml")
    }

    Dialog {
        id: poweroffDialog
        visible: false
        title: "Shutdown"
        onYes: s_os.poweroffSystem()
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
        //color: "#C5EFF7"

        onClicked: poweroffDialog.open()
    }
}
