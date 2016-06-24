import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Qt.labs.settings 1.0
import com.silk.HAL 1.0
import com.silk.Comms 1.0


Rectangle {
    id: root
    width: 320; height: 480
    color: "#2c3e50"

    TopBar {
        id: topBar
        width: parent.width
        title: "Setup"
        onBackPressed: s_menus.pop()
    }

    SideBar {
        id: sideBar
        x: 0
        anchors.top: topBar.bottom
        anchors.bottom: root.bottom
        width: 200

        MenuButton {
            id: commsSetupButton
            text: "Comms"
            icon: "qrc:/icons/ui/wifi.png"
            //color: "#bdc3c7"
            onClicked: s_menus.push("CommsSetup.qml")
            anchors.margins: 10
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
        }

        function getUAVIconName() {
            if (s_hal.uavType === HAL.UAV_NONE) return "uav.png";
            if (s_hal.uavType === HAL.UAV_MULTIROTOR) return "uav-multirotor.png";
            if (s_hal.uavType === HAL.UAV_COPTER) return "uav-copter.png";
            if (s_hal.uavType === HAL.UAV_PLANE) return "uav-plane.png";
            if (s_hal.uavType === HAL.UAV_BOAT) return "uav-boat.png";
            if (s_hal.uavType === HAL.UAV_ROVER) return "uav-rover.png";
            return "uav.png"
        }
        function getUAVConfigQMLName() {
            if (s_hal.uavType === HAL.UAV_NONE) return "UAVConfig.qml";
            if (s_hal.uavType === HAL.UAV_MULTIROTOR) return "MultirotorConfig.qml";
            if (s_hal.uavType === HAL.UAV_COPTER) return "CopterConfig.qml";
            if (s_hal.uavType === HAL.UAV_PLANE) return "PlaneConfig.qml";
            if (s_hal.uavType === HAL.UAV_BOAT) return "BoatConfig.qml";
            if (s_hal.uavType === HAL.UAV_ROVER) return "RoverConfig.qml";
            return ""
        }


        MenuButton {
            id: editConfigButton
            enabled: s_comms.connectionStatus === Comms.CONNECTED
            text: s_hal.uavType === HAL.UAV_NONE ? "New Config" : "Config";
            icon: "qrc:/icons/ui/" + parent.getUAVIconName()
            onClicked: s_menus.push(parent.getUAVConfigQMLName())
            anchors.margins: 10
            anchors.top: commsSetupButton.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
        }
        MenuButton {
            id: editNodesButton
            //enabled: s_hal.uavType !== HAL.UAV_NONE && s_comms.connectionStatus === Comms.CONNECTED
            text: "Edit Nodes"
            icon: "qrc:/icons/ui/" + parent.getUAVIconName()
            onClicked: s_menus.push("HALNodeEditor.qml")
            anchors.margins: 10
            anchors.top: editConfigButton.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
        }
    }

    Rectangle {
        id: brightness
        color: "transparent"
        anchors.left: sideBar.right
        anchors.right: parent.right
        anchors.top: topBar.bottom
        anchors.margins: 16
        height: 60
        Text {
            id: label
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            height: 30
            width: 150
            color: "#ECF0F1"
            text: "Brightness:"
        }
        Settings {
            id: settings
            category: "screen"
            property alias brightness: slider.value
        }
        Slider {
            id: slider
            anchors.left: parent.left
            anchors.top: label.bottom
            anchors.right: parent.right
            onValueChanged: {
                s_os.brightness = value
                label.text = "Brightness: " + Math.floor(value * 100) + "%"
            }
            Component.onCompleted: {
                minimumValue: 0.1 //this is here to avoid the slider emitting a value changed with the min value before restoring the saved value.
                maximumValue: 1
            }
        }

    }



//    UAVInfo {
//        id: uavInfo
//        anchors.margins: 20
//        anchors.top: brightness.bottom
//        anchors.left: sideBar.right
//        anchors.right: parent.right
//        anchors.bottom: parent.bottom
//    }

}
