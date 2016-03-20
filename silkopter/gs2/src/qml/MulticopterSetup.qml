import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import com.silk.UAVNodeEditor 1.0

Rectangle {
    id: root
    width: 800; height: 600
    color: "#2c3e50"

    TopBar {
        id: topBar
        width: parent.width
        title: "Multi Config"
        onBackPressed: s_menus.pop()
    }

    MenuButton {
        id: quadxButton
        text: "Quad X"
        icon: "qrc:/icons/ui/quadx.png"
        //color: "#bdc3c7"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: topBar.bottom
        anchors.topMargin: 100
        height: 60
        width: 200
    }

    MenuButton {
        id: quadplusButton
        text: "Quad X"
        icon: "qrc:/icons/ui/quadx.png"
        //color: "#bdc3c7"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: topBar.bottom
        anchors.topMargin: 100
        height: 60
        width: 200
    }

    UAVNodeEditor {
        anchors.top: topBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        fillColor: "green"

        ListView {
            anchors.fill: parent
            model: parent.nodeDefModel
            delegate: Rectangle {
                height: 25
                width: name.width
                Text {
                    id: name
                    text: modelData.name
                }
            }
        }
    }
}
