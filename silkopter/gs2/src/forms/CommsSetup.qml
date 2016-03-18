import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4


Rectangle {
    id: root
    width: 320; height: 480
    color: "#2c3e50"

    signal backPressed()
    signal multiConfigPressed()

    TopBar {
        id: topBar
        width: parent.width
        title: "Comms"
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
            text: s_comms.isConnected ? "Disconnect" : "Connect"
            icon: s_comms.isConnected ? "qrc:/icons/ui/connected.png" : "qrc:/icons/ui/disconnected.png"
            color: "#bdc3c7"
            onClicked: root.connectPressed()
            anchors.margins: 10
            anchors.top: parent.top
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
