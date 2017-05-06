import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtLocation 5.6
import QtPositioning 5.6

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
    }

    RowLayout {
        id: topBar
        anchors.top: parent.top
        anchors.right: parent.right
        spacing: 2

        Label {
            TextMetrics {
                id: ampsMetrics
                text: "999.9A "
            }

            Layout.preferredWidth: ampsMetrics.width
            style: Text.Outline; styleColor: "black"
            text: s_telemetry.batteryAverageCurrent.toFixed(1) + "A"
        }
        Label {
            TextMetrics {
                id: voltsMetrics
                text: "99.9V "
            }

            Layout.preferredWidth: voltsMetrics.width
            style: Text.Outline; styleColor: "black"
            text: s_telemetry.batteryAverageVoltage.toFixed(1) + "V"
        }
        ProgressBar {
            Layout.preferredWidth: 200
            value: s_telemetry.batteryCapacityLeft
            style: ProgressBarStyle {
                progress: Rectangle {
                    color: s_telemetry.batteryCapacityLeft < 0.3 ? (s_telemetry.batteryCapacityLeft < 0.2 ? "red" : "yellow") : "green"
                }
            }
        }
        Label {
            TextMetrics {
                id: percentMetrics
                text: "100% "
            }

            Layout.preferredWidth: percentMetrics.width
            style: Text.Outline; styleColor: "black"
            text: (s_telemetry.batteryCapacityLeft * 100.0).toFixed(0)  + "%"
            color: s_telemetry.batteryCapacityLeft < 0.3 ? (s_telemetry.batteryCapacityLeft < 0.2 ? "red" : "yellow") : "green"
        }
    }


//    Plugin {
//        id: osmPlugin
//        name: "osm"
//    }

//    Map {
//        anchors.top: topBar.bottom
//        anchors.bottom: parent.bottom
//        anchors.left: parent.left
//        width: 500
//        plugin: osmPlugin
//        center: s_telemetry.position
//        zoomLevel: 10
//    }
    Plugin {
        id: osmplugin

        name: "osm"
        PluginParameter { name: "osm.useragent"; value: "silkopter_gs" }
        PluginParameter { name: "osm.mapping.host"; value: "http://osm.tile.server.address/" }
        PluginParameter { name: "osm.mapping.copyright"; value: "All mine" }
        PluginParameter { name: "osm.routing.host"; value: "http://osrm.server.address/viaroute" }
        PluginParameter { name: "osm.geocoding.host"; value: "http://geocoding.server.address" }
    }
    Plugin {
        id: hereplugin

        name: "here"
        PluginParameter { name: "here.app_id"; value: "Z5ZvL9i1ZOjQhZsGxVR6" }
        PluginParameter { name: "here.token"; value: "owBddrRguz4pZxOyTlc37g" }
        PluginParameter { name: "here.mapping.cache.disk.size"; value: "300" }
        PluginParameter { name: "here.mapping.cache.memory.size"; value: "100" }
        PluginParameter { name: "here.mapping.cache.texture.size"; value: "100" }
    }

    GroupBox{
        id: mapTypeGroup
        anchors.top: parent.top
        title: "Map Types"
        ComboBox {
            //width: 150
            model: map.supportedMapTypes
            textRole: "description"
            onCurrentIndexChanged: map.activeMapType = map.supportedMapTypes[currentIndex]
        }
    }

    Map {
        id: map
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 1000
        height: 1000
        opacity: 0.7

        copyrightsVisible: false

        MapPolyline {
            id: history
            line.width: 4
            line.color: "#60FF00FF"
        }

        Connections {
            target: s_telemetry
            onPathPointAdded: {
                history.addCoordinate(point);
            }
            onPathCleared: {
                history = [];
            }
        }

        MapCircle {
            id: homePosition
            center: s_telemetry.homePosition
            radius: 3.0
            color: 'blue'
            border.width: 2
            visible: s_telemetry.homePosition.isValid
        }
        MapCircle {
            id: position
            center: s_telemetry.position
            radius: 2.0
            color: 'green'
            border.width: 2
        }

        plugin: osmplugin
        //plugin: hereplugin
        center: s_telemetry.position
        zoomLevel: 10//map.minimumZoomLevel //(map.minimumZoomLevel + map.maximumZoomLevel) / 2
        gesture.enabled: true
    }
}
