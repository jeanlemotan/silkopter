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
        title: "New Config"
        onBackPressed: s_menus.pop()
    }

    MenuButton {
        id: multirotorButton
        text: "Multirotor"
        icon: "qrc:/icons/ui/uav-multirotor.png"
        //color: "#bdc3c7"
        onClicked: {
            s_hal.createDefaultConfig(UAV.MULTIROTOR);
            //s_menus.push("MultirotorConfig.qml");
            s_menus.pop();
        }
        anchors.margins: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: topBar.bottom
        anchors.topMargin: 50
        width: 200
        height: 50
    }
    MenuButton {
        id: planeButton
        enabled: false
        text: "Plane"
        icon: "qrc:/icons/ui/uav-plane.png"
        //color: "#bdc3c7"
        //onClicked: s_menus.push("CommsSetup.qml")
        anchors.margins: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: multirotorButton.bottom
        width: 200
        height: 50
    }
    MenuButton {
        id: roverButton
        enabled: false
        text: "Rover"
        icon: "qrc:/icons/ui/uav-rover.png"
        //color: "#bdc3c7"
        //onClicked: s_menus.push("CommsSetup.qml")
        anchors.margins: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: planeButton.bottom
        width: 200
        height: 50
    }
    MenuButton {
        id: copterButton
        enabled: false
        text: "Copter"
        icon: "qrc:/icons/ui/uav-copter.png"
        //color: "#bdc3c7"
        //onClicked: s_menus.push("CommsSetup.qml")
        anchors.margins: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: roverButton.bottom
        width: 200
        height: 50
    }
    MenuButton {
        id: boatButton
        enabled: false
        text: "Boat"
        icon: "qrc:/icons/ui/uav-boat.png"
        //color: "#bdc3c7"
        //onClicked: s_menus.push("CommsSetup.qml")
        anchors.margins: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: copterButton.bottom
        width: 200
        height: 50
    }

}
