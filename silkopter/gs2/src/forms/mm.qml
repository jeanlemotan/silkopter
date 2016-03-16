import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4


Rectangle {
    id: root
    width: 800; height: 600
    color: "#2c3e50"

    signal backPressed()
    signal setupPressed()
    signal hudPressed()

    TopBar {
        id: topBar
        width: parent.width
        title: "Silkopter"
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
            id: flyButton
            enabled: s_comms.isConnected
            text: "HUD"
            icon: "qrc:/icons/ui/fly.png"
            color: "#bdc3c7"
            onClicked: root.hudPressed()
            anchors.margins: 10
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 60
        }
        MenuButton
        {
            id: setupButton
            enabled: s_comms.isConnected
            text: "Setup"
            icon: "qrc:/icons/ui/setup.png"
            color: "#bdc3c7"
            onClicked: root.setupPressed()
            anchors.margins: 10
            anchors.top: flyButton.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 60
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
