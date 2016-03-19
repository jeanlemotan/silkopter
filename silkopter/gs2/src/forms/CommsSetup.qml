import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2
import com.silk.comms 1.0


Rectangle {
    id: root
    width: 320; height: 480
    color: "#2c3e50"

    TopBar {
        id: topBar
        width: parent.width
        title: "Comms"
        onBackPressed: s_menus.pop()
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
            text: (s_comms.connectionStatus != Comms.DISCONNECTED) ? "Disconnect" : "Connect"
            icon: s_comms.isConnected ? "qrc:/icons/ui/connected.png" : "qrc:/icons/ui/disconnected.png"
            enabled: s_comms.connectionType != Comms.NONE
            //color: "#bdc3c7"
            onClicked: {
                if (s_comms.connectionStatus === Comms.DISCONNECTED) s_comms.connect();
                else s_comms.disconnect();
            }

            anchors.margins: 10
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
        }
    }

    Rectangle {
        id: configPanel
        color: "transparent"
        anchors.margins: 20
        anchors.top: topBar.bottom
        anchors.left: sideBar.right
        anchors.right: root.right
        anchors.bottom: root.bottom

        GroupBox {
            title: "Protocol"

            RowLayout {
                ExclusiveGroup { id: tabPositionGroup }
                RadioButton {
                    id: connectionTypeNoneRadio
                    text: "None"
                    exclusiveGroup: tabPositionGroup
                    onClicked: s_comms.connectionType = Comms.NONE
                }
                RadioButton {
                    id: connectionTypeUDPRadio
                    text: "UDP"
                    exclusiveGroup: tabPositionGroup
                    onClicked: s_comms.connectionType = Comms.UDP
                }
                RadioButton {
                    id: connectionTypeRFMONRadio
                    text: "RFMON"
                    exclusiveGroup: tabPositionGroup
                    onClicked: s_comms.connectionType = Comms.RFMON
                }
                Component.onCompleted: {
                    if (s_comms.connectionType === Comms.NONE) connectionTypeNoneRadio.checked = true;
                    else if (s_comms.connectionType === Comms.UDP) connectionTypeUDPRadio.checked = true;
                    else if (s_comms.connectionType === Comms.RFMON) connectionTypeRFMONRadio.checked = true;
                }
            }
        }
    }

//    UAVInfo {
//        id: uavInfo
//        anchors.margins: 20
//        anchors.top: topBar.bottom
//        anchors.left: sideBar.right
//        anchors.right: root.right
//        anchors.bottom: root.bottom
//    }


}
