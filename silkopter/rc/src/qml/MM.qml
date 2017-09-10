import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick.Extras 1.4
import QtQuick.Controls.Styles 1.4
import QtLocation 5.6
import QtPositioning 5.6

import com.silk.VideoSurface 0.1
import com.silk.Telemetry 1.0

Rectangle {
    readonly property int telemetryFontSize: 8
    readonly property int batteryBarWidth: 80
    readonly property int bigMapWidth: 200
    readonly property int bigMapHeight: 200
    readonly property int smallMapWidth: 100
    readonly property int smallMapHeight: 100
    readonly property int mapButtonSize: 32

    id: root
    color: "black"

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
        spacing: 4

        Label {
            TextMetrics {
                font.pointSize: root.telemetryFontSize;
                id: ampsMetrics
                text: "999.9A "
            }

            Layout.preferredWidth: ampsMetrics.width
            style: Text.Outline; styleColor: "black"
            font.pointSize: root.telemetryFontSize;
            text: s_telemetry.batteryAverageCurrent.toFixed(1) + "A"
        }
        Label {
            TextMetrics {
                font.pointSize: root.telemetryFontSize;
                id: voltsMetrics
                text: "99.9V "
            }

            Layout.preferredWidth: voltsMetrics.width
            style: Text.Outline; styleColor: "black"
            font.pointSize: root.telemetryFontSize;
            text: s_telemetry.batteryAverageVoltage.toFixed(1) + "V"
        }
        ProgressBar {
            Layout.preferredWidth: root.batteryBarWidth
            value: s_telemetry.batteryCapacityLeft
            style: ProgressBarStyle {
                progress: Rectangle {
                    color: s_telemetry.batteryCapacityLeft < 0.3 ? (s_telemetry.batteryCapacityLeft < 0.2 ? "red" : "yellow") : "green"
                }
            }
        }
        Label {
            TextMetrics {
                font.pointSize: telemetryFontSize;
                id: percentMetrics
                text: "100% "
            }

            Layout.preferredWidth: percentMetrics.width
            style: Text.Outline; styleColor: "black"
            font.pointSize: root.telemetryFontSize;
            text: (s_telemetry.batteryCapacityLeft * 100.0).toFixed(0)  + "%"
            color: s_telemetry.batteryCapacityLeft < 0.3 ? (s_telemetry.batteryCapacityLeft < 0.2 ? "red" : "yellow") : "green"
        }
    }
    ColumnLayout {
        id: locationInfoBar
        anchors.top: topBar.bottom
        anchors.right: parent.right
        spacing: 4

        Label {
            style: Text.Outline; styleColor: "black"
            font.pointSize: root.telemetryFontSize;
            text: "Distance From Home: " + s_telemetry.homePosition.distanceTo(s_telemetry.position).toFixed(1) + "m"
        }
        Label {
            style: Text.Outline; styleColor: "black"
            font.pointSize: root.telemetryFontSize;
            text: "Distance From Me: " + userPosition.position.coordinate.distanceTo(s_telemetry.position).toFixed(1) + "m"
        }
        Label {
            style: Text.Outline; styleColor: "black"
            font.pointSize: root.telemetryFontSize;
            text: "Altitude: " + (s_telemetry.position.altitude - s_telemetry.homePosition.altitude).toFixed(1) + "m"
        }
        Label {
            style: Text.Outline; styleColor: "black"
            font.pointSize: root.telemetryFontSize;
            text: "Horizontal Velocity: " + (s_telemetry.enuVelocity.toVector2d().length()).toFixed(1) + "m/s"
        }
        Label {
            style: Text.Outline; styleColor: "black"
            font.pointSize: root.telemetryFontSize;
            text: "Vertical Velocity: " + (s_telemetry.enuVelocity.z).toFixed(1) + "m/s"
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
        PluginParameter { name: "osm.useragent"; value: "silkopter_viewer" }
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
    Plugin {
        id: mapboxplugin

        name: "mapbox"
        PluginParameter { name: "mapbox.access_token"; value: "pk.eyJ1IjoiamVhbmxlZmxhbWJldXIiLCJhIjoiY2oyZWl5OThkMDA0bDJ3cGE1Ym9ob3ptaiJ9.Azg-At8Zzd-H98_Tuf4IwA" }
    }

//    GroupBox {
//        id: mapTypeGroup
//        anchors.top: parent.top
//        title: "Map Types"
//        ComboBox {
//            //width: 150
//            model: map.supportedMapTypes
//            textRole: "description"
//            onCurrentIndexChanged: map.activeMapType = map.supportedMapTypes[currentIndex]
//        }
//    }

    Map {
        id: map
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: root.bigMapWidth
        height: root.bigMapWidth
        //opacity: 0.7
        plugin: mapboxplugin
        zoomLevel: 10//map.minimumZoomLevel //(map.minimumZoomLevel + map.maximumZoomLevel) / 2
        gesture.enabled: true
        copyrightsVisible: false

        property int centerMapOn: 0;
        property bool freeMapPanning: false;

        Component.onCompleted: {
            var count = map.supportedMapTypes.length
            console.log('Number of supported map types: ' + count)
            console.log('Supported map types:')
            for (var i = 0; i < count; i++) {
                console.log('style: ' + map.supportedMapTypes[i].style
                          + ', name: ' + map.supportedMapTypes[i].name
                          + ', desc: ' + map.supportedMapTypes[i].description
                          + ', mobile: ' + map.supportedMapTypes[i].mobile)
                if (map.supportedMapTypes[i].style === MapType.SatelliteMapDay ||
                    map.supportedMapTypes[i].style === MapType.SatelliteMapNight)
                {
                    console.log("    satellite map type found, setting as active");
                    map.activeMapType = map.supportedMapTypes[i];
                }
            }
        }

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
            onTelemetryChanged: {
                if (map.freeMapPanning === false && map.centerMapOn === 0) {
                    map.center = s_telemetry.position;
                }
            }
        }
        Connections {
            target: map.gesture
            //unlock the centering when panning
            onPanStarted: {
                map.freeMapPanning = true;
            }
        }

        MapCircle {
            id: userPositionIndicator
            center: userPosition.position.coordinate
            radius: 2.0
            SequentialAnimation on color {
                                loops: Animation.Infinite
                                ColorAnimation { from: "#60FF0000"; to: "white"; duration: 1000 }
                                ColorAnimation { from: "white"; to: "#60FF0000"; duration: 1000 }
            }
            border.width: 2
        }
        MapCircle {
            id: homePositionIndicator
            center: s_telemetry.homePosition
            radius: 3.0
            SequentialAnimation on color {
                                loops: Animation.Infinite
                                ColorAnimation { from: "blue"; to: "white"; duration: 1000 }
                                ColorAnimation { from: "white"; to: "blue"; duration: 1000 }
            }
            border.width: 2
            visible: s_telemetry.homePosition.isValid
        }
        MapCircle {
            id: positionIndicator
            center: s_telemetry.position
            radius: 2.0
            SequentialAnimation on color {
                                loops: Animation.Infinite
                                ColorAnimation { from: "green"; to: "white"; duration: 1000 }
                                ColorAnimation { from: "white"; to: "green"; duration: 1000 }
            }
            border.width: 2
        }

        function getCenterMapOnIcon() {
            if (map.centerMapOn == 0) return "qrc:/icons/ui/uav-multirotor.png";
            if (map.centerMapOn == 1) return "qrc:/icons/ui/location.png";
            return ""
        }

        function showBigMap(show) {
            changeMapCenteringButton.visible = show;
            openBigMapButton.visible = !show;
            closeBigMapButton.visible = show;

            if (show === true) {
                map.width = root.bigMapWidth;
                map.height = root.bigMapHeight;
            } else {
                map.width = root.smallMapWidth;
                map.height = root.smallMapHeight;
            }
        }

        Button {
            id: changeMapCenteringButton
            anchors.right: map.right
            anchors.top: map.top
            width: root.mapButtonSize
            height: width

            Image {
                opacity: map.freeMapPanning === false ? 1.0 : 0.3;
                source: map.getCenterMapOnIcon();
                fillMode: Image.PreserveAspectFit
                anchors.fill: parent
                anchors.margins: 8
            }

            onClicked: {
                if (map.freeMapPanning === true) {
                    map.freeMapPanning = false;
                } else {
                    map.centerMapOn = map.centerMapOn + 1;
                    if (map.centerMapOn > 1) {
                        map.centerMapOn = 0;
                    }
                }

                if (map.centerMapOn === 0) {
                    map.center = s_telemetry.position
                } else if (map.centerMapOn === 1) {
                    map.center = userPosition.position.coordinate
                }
            }
        }
        Button {
            id: closeBigMapButton
            anchors.right: map.right
            anchors.top: changeMapCenteringButton.bottom
            width: root.mapButtonSize
            height: width

            Image {
                source: "qrc:/icons/ui/back.png"
                fillMode: Image.PreserveAspectFit
                anchors.fill: parent
                anchors.margins: 8
            }
            onClicked: {
                map.showBigMap(false);
            }
        }
        MouseArea {
            id: openBigMapButton
            anchors.fill: parent
            onClicked: {
                map.showBigMap(true);
            }
        }

        PositionSource {
            id: userPosition
            updateInterval: 1000
            active: true

            onPositionChanged: {
                if (map.freeMapPanning === false && map.centerMapOn === 1) {
                    var coord = userPosition.position.coordinate;
                    map.center = coord;
                }
            }
        }
    }
}
