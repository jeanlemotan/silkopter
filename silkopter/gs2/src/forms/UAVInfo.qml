import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Rectangle {
    id: root
    color: "#2c3e50"

    Text {
        id: statusText
        anchors.centerIn: root
        color: "#ecf0f1"
        text: "Connected: " + (s_comms.isConnected  ? (s_comms.interface + ":" + s_comms.interfaceId) : "No")
    }
    Text {
        id: interfacesText
        anchors.top: statusText.bottom
        color: "#ecf0f1"
        text: "Interfaces: " + s_comms.enumerateRFMONInterfaces
    }
}
