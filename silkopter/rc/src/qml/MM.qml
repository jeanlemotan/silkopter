import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import com.silk.Comms 1.0
import com.silk.VideoRenderer 1.0


Item {
    id: root
    width: 800; height: 600
//    color: "#2c503e"

    TopBar {
        id: topBar
        width: parent.width
        title: "Silkopter"
        onBackPressed: s_menus.pop()

        TrayButton {
            function getConnectionIcon() {
                if (s_comms.connectionStatus === Comms.CONNECTED) return "qrc:/icons/ui/connected.png";
                if (s_comms.connectionStatus === Comms.DISCONNECTED) return "qrc:/icons/ui/disconnected.png";
                return ""
            }

            id: connectionButton
            icon: getConnectionIcon()
            //color: "#bdc3c7"
            enabled: s_comms.connectionType != Comms.NONE
            onClicked: {
                if (s_comms.connectionStatus === Comms.DISCONNECTED) s_comms.connect();
                else s_comms.disconnect();
            }
            anchors.margins: 4
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.trayArea.right
            width: height

            BusyIndicator {
                visible: s_comms.connectionStatus === Comms.CONNECTING
                anchors.fill: parent
                anchors.margins: 2
                running: true
            }
        }
    }

    SideBar {
        id: sideBar
        x: 0
        anchors.top: topBar.bottom
        anchors.bottom: root.bottom
        width: 200

        MenuButton
        {
            id: hudButton
            enabled: s_comms.connectionStatus == Comms.CONNECTED
            text: "HUD"
            icon: "qrc:/icons/ui/hud.png"
            //color: "#bdc3c7"
            onClicked: s_menus.push("HUD.qml")
            anchors.margins: 10
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
        }
        MenuButton
        {
            id: setupButton
            text: "Setup"
            icon: "qrc:/icons/ui/setup.png"
            //color: "#bdc3c7"
            onClicked: s_menus.push("Setup.qml")
            anchors.margins: 10
            anchors.top: hudButton.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
        }
    }

    VideoRenderer {
        id: renderer
        //anchors.fill: parent
        anchors.top: topBar.bottom
        anchors.left: sideBar.right
        anchors.right: root.right
        anchors.bottom: root.bottom
    }

//    UAVInfo {
//        id: uavInfo
//        anchors.margins: 20
//    }

}
