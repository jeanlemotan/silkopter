import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import com.silk.Comms 1.0
import com.silk.VideoSurface 0.1
import com.silk.Telemetry 1.0

Item {
    id: root
//    width: 800; height: 600
//    color: "#2c503e"

    TrayButton {
        function getConnectionIcon() {
            if (s_comms.connectionStatus === Comms.CONNECTED) return "qrc:/icons/ui/connected.png";
            if (s_comms.connectionStatus === Comms.DISCONNECTED) return "qrc:/icons/ui/disconnected.png";
            if (s_comms.connectionStatus === Comms.CONNECTING) return "qrc:/icons/ui/reconnect.png";
            return ""
        }

        id: connectionButton
        icon: getConnectionIcon()
        enabled: true
        onClicked: {
            if (s_comms.connectionStatus === Comms.DISCONNECTED) s_comms.connect();
            else s_comms.disconnect();
        }
        anchors.margins: 4
        anchors.top: parent.top
        anchors.right: parent.right
        width: 100
        height: 100

        BusyIndicator {
            visible: s_comms.connectionStatus === Comms.CONNECTING
            anchors.fill: parent
            anchors.margins: 2
            running: true
        }
    }

    VideoSurface {
        id: renderer
        //anchors.fill: parent
        anchors.top: root.top
        anchors.left: root.left
        anchors.right: root.right
        anchors.bottom: root.bottom

        Label {
            id: telemetryVoltage
            text: "Voltage: " + s_comms.telemetry.batteryAverageVoltage + "V"
        }
        Label {
            id: telemetryCurrent
            anchors.top: telemetryVoltage.bottom
            text: "Current: " + s_comms.telemetry.batteryAverageCurrent + "A"
        }
        Label {
            id: telemetryCapacity
            anchors.top: telemetryCurrent.bottom
            text: "Capacity: " + s_comms.telemetry.batteryCapacityLeft * 100  + "%"
        }
    }

//    UAVInfo {
//        id: uavInfo
//        anchors.margins: 20
//    }

}
