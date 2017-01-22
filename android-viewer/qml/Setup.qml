import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Qt.labs.settings 1.0
import com.silk.HAL 1.0
import com.silk.Comms 1.0
import com.silk.VideoRenderer 1.0


Item {
    id: root
    width: 800; height: 480
    //color: "#2c3e50"

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
        width: topBar.height * (root.width / root.height)

        MenuButton {
            id: commsSetupButton
//            text: "Comms"
            icon: "qrc:/icons/ui/wifi.png"
            //color: "#bdc3c7"
            onClicked: s_menus.push("CommsSetup.qml")
            anchors.margins: 10
            anchors.top: parent.top
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

    VideoRenderer {
        id: renderer
        anchors.top: topBar.bottom
        anchors.left: sideBar.right
        anchors.right: root.right
        anchors.bottom: root.bottom
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
