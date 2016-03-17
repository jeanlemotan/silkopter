import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4


Rectangle {
    id: root
    width: 320; height: 480
    color: "#2c3e50"

    signal backPressed()
    signal multicopterPressed()
    signal roverPressed()
    signal planePressed()
    signal boatPressed()
    signal commsPressed()

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
            id: multicopterButton
            text: "Multicopter"
            icon: "qrc:/icons/ui/uav-multicopter.png"
            color: "#bdc3c7"
            onClicked: root.multicopterPressed()
            anchors.margins: 10
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
        }
        MenuButton
        {
            id: roverButton
            text: "Rover"
            icon: "qrc:/icons/ui/uav-rover.png"
            color: "#bdc3c7"
            onClicked: root.roverPressed()
            anchors.margins: 10
            anchors.top: multicopterButton.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
        }
        MenuButton
        {
            id: planeButton
            text: "Plane"
            icon: "qrc:/icons/ui/uav-plane.png"
            color: "#bdc3c7"
            onClicked: root.planePressed()
            anchors.margins: 10
            anchors.top: roverButton.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
        }
        MenuButton
        {
            id: boatButton
            text: "Boat"
            icon: "qrc:/icons/ui/uav-boat.png"
            color: "#bdc3c7"
            onClicked: root.boatPressed()
            anchors.margins: 10
            anchors.top: planeButton.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
        }
        MenuButton
        {
            id: commsButton
            text: "Comms"
            icon: "qrc:/icons/ui/wifi.png"
            color: "#bdc3c7"
            onClicked: root.commsPressed()
            anchors.margins: 10
            anchors.top: boatButton.bottom
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
