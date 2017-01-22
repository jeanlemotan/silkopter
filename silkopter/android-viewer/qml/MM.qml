import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import com.silk.Comms 1.0
import com.silk.VideoSurface 0.1

Item {
    id: root
//    width: 800; height: 600
//    color: "#2c503e"

    TopBar {
        id: topBar
        height: 100
        width: parent.width
        title: "Slkptr"
        onBackPressed: s_menus.pop()

        TrayButton {
            function getConnectionIcon() {
                if (s_comms.connectionStatus === Comms.CONNECTED) return "qrc:/icons/ui/connected.png";
                if (s_comms.connectionStatus === Comms.DISCONNECTED) return "qrc:/icons/ui/disconnected.png";
                if (s_comms.connectionStatus === Comms.CONNECTING) return "qrc:/icons/ui/reconnect.png";
                return ""
            }

            id: connectionButton
            icon: getConnectionIcon()
            //color: "#bdc3c7"
            enabled: true
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
        width: root.width / 10

        MenuButton
        {
            id: hudButton
            enabled: s_comms.connectionStatus == Comms.CONNECTED
            //text: "HUD"
            icon: "qrc:/icons/ui/hud.png"
            //color: "#bdc3c7"
            onClicked: s_menus.push("HUD.qml")
            anchors.margins: 10
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: root.height / 10
        }
        MenuButton
        {
            id: setupButton
            //text: "Setup"
            icon: "qrc:/icons/ui/setup.png"
            //color: "#bdc3c7"
            onClicked: s_menus.push("Setup.qml")
            anchors.margins: 10
            anchors.top: hudButton.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: root.height / 10
        }
    }

    VideoSurface {
        id: renderer
        //anchors.fill: parent
        anchors.top: topBar.bottom
        anchors.left: sideBar.right
        anchors.right: root.right
        anchors.bottom: root.bottom

        MouseArea {
            property bool fullscreen: false;
            anchors.fill: parent
            onClicked: {
                if (fullscreen)
                {
                    sideBar.visible = true
                    topBar.visible = true

                    renderer.anchors.top = topBar.bottom
                    renderer.anchors.left = sideBar.right
                    renderer.anchors.right = root.right
                    renderer.anchors.bottom = root.bottom
                    fullscreen = false;
                }
                else
                {
                    sideBar.visible = false
                    topBar.visible = false

                    renderer.anchors.top = root.top
                    renderer.anchors.left = root.left
                    renderer.anchors.right = root.right
                    renderer.anchors.bottom = root.bottom
                    fullscreen = true;
                }
            }
        }

    }

//    UAVInfo {
//        id: uavInfo
//        anchors.margins: 20
//    }

}
