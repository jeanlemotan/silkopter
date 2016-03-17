import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4


Rectangle {
    id: root
    width: 320; height: 480
    color: "#2c3e50"

    signal backPressed()
    signal multiConfigPressed()
    signal commsConfigPressed()

    TopBar {
        id: topBar
        width: parent.width
        title: "Setup"
        onBackPressed: root.backPressed()
    }

    SideBar {
        id: sideBar
        x: 0
        anchors.top: topBar.bottom
        anchors.bottom: root.bottom
        width: 200

        MenuButton
        {
            id: multiConfigButton
            text: "Multi Config"
            icon: "qrc:/icons/ui/quad.png"
            color: "#bdc3c7"
            onClicked: root.multiConfigPressed()
            anchors.margins: 10
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
        }
        MenuButton
        {
            id: commsConfigButton
            text: "Comms Config"
            icon: "qrc:/icons/ui/wifi.png"
            color: "#bdc3c7"
            onClicked: root.commsConfigPressed()
            anchors.margins: 10
            anchors.top: flyButton.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
        }
    }

    UAVInfo {
        id: uavInfo
        anchors.margins: 20
        anchors.top: topBar.bottom
        anchors.left: sideBar.right
        anchors.right: root.right
        anchors.bottom: root.bottom
    }

}
