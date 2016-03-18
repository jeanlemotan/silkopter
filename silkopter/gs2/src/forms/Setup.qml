import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Qt.labs.settings 1.0


Rectangle {
    id: root
    width: 320; height: 480
    color: "#2c3e50"

    signal backPressed()
    signal multicopterSetupPressed()
    signal roverSetupPressed()
    signal planeSetupPressed()
    signal boatSetupPressed()
    signal commsSetupPressed()

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

        MenuButton {
            id: multicopterSetupButton
            text: "Multicopter"
            icon: "qrc:/icons/ui/uav-multicopter.png"
            color: "#bdc3c7"
            onClicked: root.multicopterSetupPressed()
            anchors.margins: 10
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
        }
        MenuButton {
            id: roverSetupButton
            text: "Rover"
            icon: "qrc:/icons/ui/uav-rover.png"
            color: "#bdc3c7"
            onClicked: root.roverSetupPressed()
            anchors.margins: 10
            anchors.top: multicopterSetupButton.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
        }
        MenuButton {
            id: planeSetupButton
            text: "Plane"
            icon: "qrc:/icons/ui/uav-plane.png"
            color: "#bdc3c7"
            onClicked: root.planeSetupPressed()
            anchors.margins: 10
            anchors.top: roverSetupButton.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
        }
        MenuButton {
            id: boatSetupButton
            text: "Boat"
            icon: "qrc:/icons/ui/uav-boat.png"
            color: "#bdc3c7"
            onClicked: root.boatSetupPressed()
            anchors.margins: 10
            anchors.top: planeSetupButton.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
        }
        MenuButton {
            id: commsSetupButton
            text: "Comms"
            icon: "qrc:/icons/ui/wifi.png"
            color: "#bdc3c7"
            onClicked: root.commsSetupPressed()
            anchors.margins: 10
            anchors.top: boatSetupButton.bottom
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
        height: 100
        Text {
            id: label
            anchors.top: parent.top
            anchors.left: parent.left
            height: 30
            width: 150
            color: "#bdc3c7"
            text: "Brightness:"
        }
        Settings {
            id: settings
            category: "screen"
            property alias brightness: slider.value
        }
        Slider {
            id: slider
            anchors.left: label.right
            anchors.top: parent.top
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
